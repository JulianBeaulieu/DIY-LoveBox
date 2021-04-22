var pubKey = "pub-c-4f10f9cd-acff-4f28-99aa-5cf1889d9100";
var subKey = "sub-c-bfa9cc1c-91cf-11eb-a414-3ac7414c19a1";
var channels = "julia";

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
