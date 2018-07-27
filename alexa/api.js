const axios = require('axios'); 
const config = require('./config.js');

const http = axios.create(config);

function status() {
    return http.get('/isClosed').then((res) => {
        let status = res.data && res.data.isClosed;
        if(status) {
            return "Your garage is closed";
        } else {
            return "Your garage is open";
        }
    }).catch((e) => {
        console.log(e);
        return "Couldn't get the status from your garage";
    });
}

async function toggle(status) {
    return http.get('/toggle').then((res) => {
        if(status == 'open') {
            return "Opening your garage";
        } else {
            return "Closing your garage";
        }
    }).catch((e) => {
        console.log(e);
        return "Couldn't close or open your garage";
    })      
}

module.exports = {
    status,
    toggle
};
