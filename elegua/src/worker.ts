//
// Created by Long  on 18/11/2023.
//

// The main object will be a class that contains fields such as the state, the ID of the telegram
// chat that the bot uses, and the security token of the bot.
let lastPingTimestamp = 0;

export default {
    chatID: '',             // read from the bot message
    bot_token: '',          // read from the worker environment variables
    KV: null,

    // This will receive the scheduled ping event
    async scheduled(event, env, ctx) {
        this.KV = env.KV_ROVER;
        console.log("received a cron request");
        const state = await this.KV.get("state");
        if (state === "off") {
            console.log("The bot is off, no need to check if it is still alive");
            return null;
        }

        this.bot_token = env.BOT_TOKEN;
        this.chatID = env.CHAT_ID;

        await this.sendTelegramMessage("Worker executed a cron event");
        ctx.waitUntil(this.checkRoverAlive());
    },

	// this is the entrypoint for the Cloudflare worker. All requests will be processed by this.
    async fetch(request, env) {
        this.KV = env.KV_ROVER;

        // Handle preflight OPTIONS request - used to allow requests incoming
        // from the editor.swagger.io page
        if (request.method === 'OPTIONS') {
            return optionsRequest();
        }

        this.bot_token = env.BOT_TOKEN;
        if (this.bot_token === '') {
            return this.errNoToken();
        }

        const { pathname } = new URL(request.url);

        if (request.body == null) {
            return this.errNoBody();
        }

        const body = await request.json();
        this.chatID = extractChatIdFromBody(body);

        if (body && body.message && body.message.text) {
            console.log(`here is the chat text: ${body.message.text}`);
        } else {
            console.log("One of the objects is not defined.");
            await this.sendTelegramMessage("Could not read text of the message.")
            return new Response('Could not read text of the message');
        }

        return this.routeAndProcessRequest(body);
    },

    async routeAndProcessRequest(body: any) {
        console.log("Switch: " + body.message.text);
        switch(body.message.text) {
						case "state": case "return state":
								return this.returnState();
						case "turn on":
								return this.turnOn(body);
						case "turn off":
								return this.turnOff();
						case "ping":
								return this.ping();
						case "forward": case "backward": case "left": case "right":
								return this.storeMovement(body.message.text);
						case "photo":
								return this.storeMovement("photo");
						case "list":
								return this.listMovements();
						case "clear":
								return this.clearBuffer();
            default:
                await this.sendTelegramMessage(`I don't understand what you want: "${body.message.text}'`)
        }

        return new Response("I don't understand what you want.");
    },

		async clearBuffer() {
				const datos = [];
				const buffer = new TextEncoder().encode(JSON.stringify(datos)).buffer;
				this.KV.put('movements', buffer);

				await this.sendTelegramMessage('The buffer was cleared');
				return okResponse();
		},

		async storeMovement(command: string) {
			const oldList = await this.KV.get('movements', 'arrayBuffer');
			const stringList= new TextDecoder().decode(new Uint8Array(oldList));
			const arrayList = JSON.parse(stringList);

			arrayList.push(command)
			const buffer = new TextEncoder().encode(JSON.stringify(arrayList)).buffer;
			await this.KV.put('movements', buffer);

			await this.sendTelegramMessage(`Command stored: ${command}`);
			return okResponse();
		},

		async listMovements() {
				const movements = await this.KV.get('movements', 'arrayBuffer');
				const list= new TextDecoder().decode(new Uint8Array(movements));

				await this.sendTelegramMessage(`${list}`);
				return jsonResponse({'message': 'ok'});
		},

    async returnState() {
        const now = new Date();
        const state = await this.KV.get("state");

        await this.sendTelegramMessage(`The rover is: ${state}. [${now.toLocaleString()}]`);
        return jsonResponse({'state': state, 'message': `The rover is: ${state}`});
    },

    async turnOn(body: any) {
        const timestamp = Date.now();
        const now = new Date(timestamp);

        Promise.all([
            this.KV.put("state", "on"),
            this.KV.put("last_ping_timestamp", timestamp),
            this.sendTelegramMessage(`The rover was turned on [${now.toLocaleString()}]. ${body.message.info ?? ''}`),
        ]);

        console.log(`I set the last ping timestamp to: ${timestamp}`);

        return jsonResponse({"state": "on", "message": "The rover was turned on"});
    },

    async turnOff() {
        const timestamp = Date.now();
        const now = new Date(timestamp);

        await Promise.all([
            this.KV.put("last_ping_timestamp", 0),
            this.KV.put("state", "off"),
            this.sendTelegramMessage(`The rover was turned off [${now.toLocaleString()}]`),
        ]);

        return jsonResponse({"state:": "off", "message": "The rover was turned off"});
    },

    async ping() {
        const timestamp = Date.now();
        const now = new Date();

        // await Promise.all([
            // this.KV.put("state", "on"),
            // this.KV.put("last_ping_timestamp", timestamp),
            // this.sendTelegramMessage(`[x] The rover 1 sent a ping [${now.toLocaleString()}]`),
        // ])

        return new Response("Ok");
    },

    async sendTelegramMessage(message) {
        const apiUrl = `https://api.telegram.org/bot${this.bot_token}/sendMessage`;

        try {
            console.log(`Fetching: ${apiUrl}`);
            console.log(`Message: ${message}`);
            const response = await fetch(apiUrl, {
                method: 'POST',
                headers: {  'Content-Type': 'application/json' },
                body: JSON.stringify({
                    chat_id: this.chatID,
                    text: message
                })
            })

            const responseBody = await response.text();
            console.log('Telegram API Response:', responseBody);
        } catch (e) {
            console.log("Something went wrong calling the Telegram API: " + e);
        }
    },

    async checkRoverAlive() {
        const lastPingTimestamp = await this.KV.get("last_ping_timestamp")

        console.log("check if rover is alive");
        console.log(`Last Ping Timestamp: ${lastPingTimestamp}`);

        const timestamp = Date.now();
        const now = new Date(timestamp);

        const inactivityDuration = timestamp - lastPingTimestamp;
        console.log(`Inactive for: ${inactivityDuration} milliseconds`);

        // Assuming 20 seconds as the threshold for inactivity (adjust as needed)
        if (inactivityDuration >= 20000) { // one minute, in milliseconds
            console.log("the rover has been inactive for more than 20 seconds");
            await this.KV.put("state", "off");
            await this.KV.put("last_ping_timestamp", 0);
            await this.sendTelegramMessage(`The bot was turned off after a period of inactivity [${now.toLocaleString()}]`);
            console.log("Robot state set to off due to inactivity");
        }
    },

		async errNoToken() {
			await this.sendTelegramMessage("Missing bot token. Check the configuration of your Cloudflare worker.");
		},

		async errNoBody() {
			await this.sendTelegramMessage("Missing request body.");
		}
} // end of class

// The following functions don't require any information from the worker so they can be declared
// outside of the object

function okResponse() {
	return new Response("OK", {
		headers: {
			'Content-Type': 'application/json',
		},
		status: 200,
		statusText: "OK"
	});
}

function jsonResponse(responseObject) {
    const jsonResponse = JSON.stringify(responseObject);

    return new Response(jsonResponse, {
        headers: {
            'Content-Type': 'application/json',
        },
    });
}

function extractChatIdFromBody(body) {
    return body.message && body.message.chat && body.message.chat.id;
}


function optionsRequest() {
    return new Response(null, {
        headers: {
            'Access-Control-Allow-Origin': '*',
            'Access-Control-Allow-Methods': 'GET, POST, OPTIONS',
            'Access-Control-Allow-Headers': 'Content-Type',
        },
    });
}
