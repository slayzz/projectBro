if (!window.WebSocket) {
	document.body.innerHTML = 'WebSocket в этом браузере не поддерживается.';
}

// создать подключение
var socket = new WebSocket("ws://localhost:8000");

socket.onopen = function() {
 var client = {
 	id: "clientID",
 	lastmessage: "HeyBro",
 	type: "firstconnection" 
 }
  


// отправить сообщение из формы publish
document.forms.publish.onsubmit = function() {
  

  socket.send(JSON.stringify(client));
  return false;
};

// обработчик входящих сообщений
socket.onmessage = function(event) {
  var incomingMessage = JSON.parse(event.data);
  showMessage(incomingMessage.lastmessage); 
  console.log(incomingMessage.lastmessage)
};

// показать сообщение в div#subscribe
function showMessage(message) {
  var messageElem = document.createElement('div');
  messageElem.appendChild(document.createTextNode(message));
  document.getElementById('subscribe').appendChild(messageElem);
}
};