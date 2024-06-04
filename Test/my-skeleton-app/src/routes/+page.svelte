<script>
    // Importing necessary components and functions
    import Buttons from '../components/Buttons.svelte';  
    import { onSend, startConnect } from '../stores/mqttStore'; 
    
    // Variables to store message data
    let messageToSend = ''; 
    let receivedMessages = []; 
    
    // Function to send a message
    function sendMessage() {
        onSend('logger', messageToSend); 
        receivedMessages = [...receivedMessages, messageToSend]; 
        messageToSend = ''; // Resetting the message to send
    }
</script>
<Buttons/>
