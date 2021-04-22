var pubKey = "pub key";
var subKey = "sub key";
var channels = "channel";

const pubnub = new PubNub({
  publishKey: ,
  subscribeKey: ,
  uuid: "Web"
});

function sendMessageToPubNub(message){
  pubnub.publish({
    channel : channels,
    message : { "Device": "Web", "Message": message}
  },
  function(status, response) {
    if (status.error) {
      console.log(status)
    }
  });
};
