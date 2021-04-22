
function options(){
  document.getElementById("action_menu").toggle();
}

function send(){
  var message = getMessageFromTextBox();

  if(isNotEmptyString(message)){
    postMessageOnWebsite(message);

    sendMessageToPubNub(message);
  }
};

function getMessageFromTextBox(){
  var message = document.getElementById("message-text-area").value.replaceAll('\n', '');
  document.getElementById("message-text-area").value = "";
  return message;
}

function isNotEmptyString(message){
  return !(message.trim() === "");
}

function postMessageOnWebsite(message){
  var messagesDiv = document.getElementById('chat-area');

  document.getElementById("chat-area").innerHTML += newMessage(message);
}

function newMessage(message){
  var message = "<div class=\"d-flex justify-content-end mb-4\">\n" +
                  "<div class=\"msg_cotainer_send\">\n" +
                    message + "\n" +
                    //"<span class=\"msg_time_send\">9:10 AM, Today</span>\n" +
                  "</div>\n" +
                "</div>"
  return message;
};
