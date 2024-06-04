// Importing MQTT library and store module
import mqtt from 'mqtt';
import { writable } from 'svelte/store';

// Store to track connection status
export const isConnected = writable(false);

// Initializing variables
let connected = false; 
let User = { 
    address: "maqiatto.com", 
    port: "8883", 
    clientId: "clientID_3", 
    name: "alex.widell.orton@hitachigymnasiet.se", 
    password: "Forre" 
};
let clientId = "notyetAssigned"; 
let client = null;
let options = {}; 

/**
 * Disconnects the MQTT client.
 */
export function startDisconnect() {
    if (client) {
        client.end();
        connected = false;
        isConnected.set(false);
    }
}

/**
 * Connects the MQTT client.
 * @returns {string} Returns an empty string if already connected.
 */
export function startConnect() {
    if (connected) {
        return ""; 
    }
    clientId = `clientID_${Math.random().toString(16).substr(2, 8)}`; // Generating a random client ID
    const mqtt_url = User.address; 
    const url = `mqtt://${mqtt_url}:${User.port}`; 
    options = { 
        clientId: clientId,
        username: User.name,
        password: User.password
    };
    client = mqtt.connect(url, options); 
    client.on('message', onMessage); 

    // Event listeners for MQTT client
    client
        .on("connect", function () { 
            connected = true;
            console.log(`${clientId} success`); 
            Connecting(true); 
        })
        .on("error", function () { 
            console.log("error"); 
            Connecting(false); 
        })
        .on("close", function () { 
            connected = false;
            Connecting(false); 
            console.log("closing"); 
        });
}

/**
 * Handles the incoming MQTT message.
 * @param {string} topic
 * @param {Buffer} message 
 */
const onMessage = (topic, message) => {
    console.log(message.toString()); 
};

/**
 * Sets the connection status and sends a log message.
 * @param {boolean} c 
 */
function Connecting(c) {
    isConnected.set(c); 
    if (c) {
        onSend("banan", `${clientId} has connected.`);
    } else {
        onSend("banan", `${clientId} has disconnected.`);
    }
}

/**
 * Publishes a message to the MQTT broker.
 * @param {string} address 
 * @param {string} message 
 */
export function onSend(address, message) {
    console.log(`${User.name}/${address}: ${message}`); 
    if (client && connected) {
        client.publish(
            `${User.name}/${address}`,
            message
        );
    }
}
