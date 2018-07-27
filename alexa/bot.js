const botBuilder = require('claudia-bot-builder')
const api = require('./api')

isIntent = function (alexaPayload, name) {
    return alexaPayload &&
    alexaPayload.request &&
    alexaPayload.request.type === 'IntentRequest' &&
    alexaPayload.request.intent &&
    alexaPayload.request.intent.name == name;
};

const bot = botBuilder(
function (message, originalRequest) {
    let body = originalRequest.body;
    if(isIntent(body, 'statusIntent')) {
        return api.status();
    } else if (isIntent(body, 'toggleIntent')) {
        return api.toggle(body.request.intent.slots.change.value);
    } else if(isIntent(body, 'AMAZON.StopIntent')) {
        return api.toggle('close');
    } else {
        return "Intent not detected";
    }
},
{ platforms: ['alexa'] }
);

module.exports = bot;