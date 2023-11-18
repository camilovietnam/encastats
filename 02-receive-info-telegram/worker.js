//
// Created by Long  on 18/11/2023.
//

// The main object will be a class that contains fields such as the state, the ID of the telegram
// chat that the bot uses, and the security token of the bot.
export default {
    state: "off",   // 'on' or 'off'
    chatID: '',     // read from the bot message
    bot_token: '',  // read from the worker environment variables

    async fetch(request, env, ctx) {
        // Handle preflight OPTIONS request - used to allow requests incoming
        // from the editor.swagger.io page
        if (request.method === 'OPTIONS') {
            return optionsRequest();
        }

        this.bot_token = env.BOT_TOKEN;
        if (this.bot_token === '') {
            return errNoToken();
        }

        const { pathname } = new URL(request.url);

        if (request.body == null) {
            return errNoBody();
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

        return this.routeAndProcessRequest(body.message.text);
    },

    async routeAndProcessRequest(requestType) {
        switch(requestType) {
            case "state": case "return state":
                return this.returnState();
            case "turn on":
                return this.turnOn();
            case "turn off":
                return this.turnOff();
            default:
                await this.sendTelegramMessage("I don't understand what you want.")
        }

        return new Response("I don't understand what you want.");
    },

    async returnState() {
        await this.sendTelegramMessage(`The rover is: ${this.state}.`);
        return jsonResponse({"state": this.state, "message": `The rover is: ${this.state}`});
    },

    async turnOn() {
        this.state = "on";
        await this.sendTelegramMessage("The rover was turned on.");
        return jsonResponse({"state": this.state, "message": "The rover was turned on"});
    },

    async turnOff() {
        this.state = "off";
        await this.sendTelegramMessage("The rover was turned off.");
        return jsonResponse({"state:": this.state, "message": "The rover was turned off"});
    },

    async sendTelegramMessage(message) {
        const apiUrl = `https://api.telegram.org/bot${this.bot_token}/sendMessage`;

        try {
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
    }
}

// The following functions don't require any information from the worker

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

async function errNoToken() {
    await this.sendTelegramMessage("Missing bot token. Check the configuration of your Cloudflare worker.");
    return jsonResponse({"state": this.state});
}

async function errNoBody() {
    await this.sendTelegramMessage("Missing request body.");
    return jsonResponse({"state": this.state});
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
