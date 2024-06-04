<script>
    // Importing necessary functions and components from mqttStore.js
    import { onSend, startConnect, startDisconnect, isConnected } from '../stores/mqttStore';
    import Page1 from './Page1.svelte';

    // Define variables to track current state
    let currentPage = 'home'; 
    let pressedKey = null; 
    let isRecording = false; 
    let recordedActions = []; 
    let downInterval = null; 

    // Log the current connection status whenever it changes
    $: console.log('Current connection status:', $isConnected);

    // Function to handle navigation between pages
    function handleNavigation(page) {
        currentPage = page;
    }

    // Function to send MQTT message
    function sendMessage(message) {
        onSend('banan', message); // Publish the message to MQTT topic

        // If recording, store the action
        if (isRecording && message) {
            recordedActions.push({ direction: message, timestamp: Date.now() });
        }
    }

    // Function to handle keydown events
    function handleKeydown(event) {
        const key = event.code.toLowerCase(); // Use event.code to handle key events

        // Handle movement keys
        if (key === 'keyw' && pressedKey !== 'w') {
            pressedKey = 'w';
            sendMessage('up');
        } else if (key === 'keya' && pressedKey !== 'a') {
            pressedKey = 'a';
            sendMessage('left');
        } else if (key === 'keys' && pressedKey !== 's') {
            pressedKey = 's';
            sendMessage('down');
            startDownInterval();
        } else if (key === 'keyd' && pressedKey !== 'd') {
            pressedKey = 'd';
            sendMessage('right');
        } else if (key === 'keyr') { // Toggle recording when 'r' key is pressed
            toggleRecording();
        }
    }

    // Function to handle keyup events
    function handleKeyup(event) {
        const key = event.code.toLowerCase();

        // Stop movement when key is released
        if ((key === 'keyw' && pressedKey === 'w') || (key === 'keys' && pressedKey === 's')) {
            pressedKey = null;
            sendMessage('stop_vertical');
            clearInterval(downInterval);
        } else if ((key === 'keya' && pressedKey === 'a') || (key === 'keyd' && pressedKey === 'd')) {
            pressedKey = null;
            sendMessage('stop_horizontal');
        }
    }

    // Function to start recording
    function startRecording() {
        isRecording = true;
        recordedActions = [];
        console.log('Recording started');
    }

    // Function to stop recording and replay the actions
    function stopAndReplayRecording() {
        isRecording = false;
        console.log('Recording stopped');
        replayActions();
    }

    // Function to replay recorded actions
    function replayActions() {
        if (recordedActions.length === 0) return;

        let startTime = recordedActions[0].timestamp;

        recordedActions.forEach((action, index) => {
            let delay = action.timestamp - startTime;
            setTimeout(() => {
                sendMessage(action.direction);
                if (index === recordedActions.length - 1) {
                    setTimeout(() => sendMessage('stop'), 500); // Stop after the last action
                }
            }, delay);
        });
    }

    // Toggle recording function called by the button
    function toggleRecording() {
        if (!isRecording) {
            startRecording();
        } else {
            stopAndReplayRecording();
        }
    }

    // Function to continuously send "down" message while S key is held down
    function startDownInterval() {
        downInterval = setInterval(() => {
            if (pressedKey === 's') {
                sendMessage('down');
            }
        }, 100); 
    }

    // Function to handle connect/disconnect toggle
    function toggleConnection() {
        if ($isConnected) {
            console.log('Disconnecting...');
            startDisconnect();
        } else {
            console.log('Connecting...');
            startConnect();
        }
    }
</script>

<!-- CSS Styles -->
<style>
    /* Button styling */
    .btn {
        padding: 50px 60px; 
        margin: 2px;
        background-color: white;
        border: 2px solid #000000;
        color: #000000;
        cursor: pointer;
        transition: transform 0.1s ease;
        border-radius: 25px; 
    }
    
    /* Page button styling */
    .page {
        padding: 20px 40px; 
        margin: 2px;
        background-color: white;
        border: 4px solid #000000;
        color: #000000;
        cursor: pointer;
        transition: transform 0.1s ease;
        border-radius: 25px;
    }

    /* Styling for pressed buttons */
    .btn.pressed {
        transform: translateY(2px);
    }

    /* Dropdown styling */
    .dropdown {
        position: relative;
        display: inline-block;
    }

    /* Dropdown content styling */
    .dropdown-content {
        display: none;
        position: absolute;
        background-color: #f9f9f9;
        min-width: 160px;
        box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);
        z-index: 1;
        border-radius: 10px; 
        overflow: hidden;
    }

    /* Show dropdown content on hover */
    .dropdown:hover .dropdown-content {
        display: block;
    }

    /* Dropdown button styling */
    .dropdown-content button {
        color: black;
        padding: 12px 16px;
        text-decoration: none;
        display: block;
        background: white;
        border: none;
        width: 100%;
        text-align: left;
        cursor: pointer;
    }

    /* Hover styling for dropdown buttons */
    .dropdown-content button:hover {
        background-color: #ddd;
    }

    /* Text color styling */
    .text-green-500 {
        color: green;
    }

    .text-red-500 {
        color: red;
    }

    /* Container styling */
    .container {
        display: flex;
        justify-content: center;
        flex-direction: column;
        align-items: center;
        height: 80vh; 
        position: relative;
        margin-top: 0px; 
    }

    /* Controls styling */
    .controls {
        display: flex;
        margin: 0px auto; 
        flex-direction: column;
        align-items: center;
        font-size: 25px;
    }

    /* Horizontal button styling */
    .horizontal-buttons {
        display: flex;
        justify-content: space-between;
        width: 100%;
    }

    /* Header styling */
    .header {
        display: flex;
        justify-content: space-between;
        align-items: center;
        padding: 10px;
        background-color: #000000;
        color: white;
        width: 100%;
    }

    /* Record button styling */
    .record-button {
        position: absolute;
        bottom: 0px; 
        right: 20px; 
    }

    /* Connect button styling */
    .connect-button {
        position: absolute;
        bottom: 0px; 
        left: 20px; 
    }

    /* Status box styling */
    .status-box {
        position: absolute;
        top: 0px; 
        right: 0px; 
        padding: 10px;
        border: 2px solid #000000;
        border-radius: 10px;
        background-color: white;
    }
</style>

<div class="header">
    <div class="dropdown">
        <button class="page">Pages</button>
        <div class="dropdown-content">
            <!-- Buttons to navigate to different pages -->
            <button on:click={() => handleNavigation('home')}>Home</button>
            <button on:click={() => handleNavigation('page1')}>About</button>
        </div>
    </div>
</div>

<!-- Conditional rendering of page content based on currentPage variable -->
{#if currentPage === 'home'}
    <div class="container">
        <!-- Button to connect/disconnect MQTT -->
        <button class="btn connect-button {($isConnected ? 'pressed' : '')}" on:click={toggleConnection}>
            {#if $isConnected}
                Disconnect
            {:else}
                Connect
            {/if}
        </button>
        <!-- Button to start/stop recording -->
        <button class="btn record-button" on:click={toggleRecording}>
            {isRecording ? 'Stop & Replay' : 'Record'}
        </button>
        <div class="controls">
            <!-- Buttons for controlling movement -->
            <button class="btn {pressedKey === 'w' ? 'pressed' : ''}" on:click={() => sendMessage('up')}>W</button> 
            <div class="horizontal-buttons">
                <button class="btn {pressedKey === 'a' ? 'pressed' : ''}" on:click={() => sendMessage('left')}>A</button> 
                <button class="btn {pressedKey === 's' ? 'pressed' : ''}" on:click={() => sendMessage('down')}>S</button> 
                <button class="btn {pressedKey === 'd' ? 'pressed' : ''}" on:click={() => sendMessage('right')}>D</button> 
            </div>
        </div>
        <!-- Display MQTT connection status -->
        <div class="status-box">
            {#if $isConnected}
                <p class="text-green-500">Connected</p> 
            {:else}
                <p class="text-red-500">Not Connected</p> 
            {/if}
        </div>
    </div>
{:else if currentPage === 'page1'}
    <!-- Render Page1 component -->
    <Page1 />
{/if}

<!-- Event listeners for keydown and keyup events -->
<svelte:window on:keydown={handleKeydown} on:keyup={handleKeyup} />
