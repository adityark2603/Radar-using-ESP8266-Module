/*Created By Electrical Mandir
Buy Project or Project Kit Whatsapp us at +91-8073420182
Subscribe our Youtube Channel for latest updates...*/

// Paste this url in Additional Board Manager url:- http://arduino.esp8266.com/stable/package_esp8266com_index.json
//install esp8266 board from board manager
//install library

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

#define TRIG D5
#define ECHO D6
#define SERVO_PIN D4
#define BUZZER D7
#define LED D8

ESP8266WebServer server(80);
Servo radarServo;

int angle = 0;
int dir = 1;
long distanceCM = 0;

/* ===================== WEB PAGE ===================== */

const char webpage[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>

<head>
<title>Radar System</title>

<style>
body{
margin:0;
background:black;
color:rgb(98,245,31);
font-family:Arial;
}

canvas{
display:block;
margin:auto;
}

</style>
</head>

<body>

<canvas id="radar" width="1200" height="700"></canvas>

<script>

const canvas=document.getElementById("radar");
const ctx=canvas.getContext("2d");

let iAngle=0;
let iDistance=0;

const centerX=canvas.width/2;
const centerY=canvas.height-canvas.height*0.074;
const maxRange=canvas.width/2;

function fadeBackground(){

ctx.fillStyle="rgba(0,0,0,0.04)";
ctx.fillRect(0,0,canvas.width,canvas.height);

}

function drawRadar(){

ctx.strokeStyle="rgb(98,245,31)";
ctx.lineWidth=2;

ctx.beginPath();

ctx.arc(centerX,centerY,maxRange,Math.PI,2*Math.PI);
ctx.arc(centerX,centerY,maxRange*0.73,Math.PI,2*Math.PI);
ctx.arc(centerX,centerY,maxRange*0.52,Math.PI,2*Math.PI);
ctx.arc(centerX,centerY,maxRange*0.31,Math.PI,2*Math.PI);

ctx.stroke();

for(let a=0;a<=180;a+=30){

let rad=a*Math.PI/180;

ctx.beginPath();
ctx.moveTo(centerX,centerY);

ctx.lineTo(
centerX-maxRange*Math.cos(rad),
centerY-maxRange*Math.sin(rad)
);

ctx.stroke();

}

ctx.beginPath();
ctx.moveTo(centerX-maxRange,centerY);
ctx.lineTo(centerX+maxRange,centerY);
ctx.stroke();

}

function drawLine(){

let rad=iAngle*Math.PI/180;

ctx.strokeStyle="rgb(30,250,60)";
ctx.lineWidth=9;

ctx.beginPath();
ctx.moveTo(centerX,centerY);

ctx.lineTo(
centerX+maxRange*Math.cos(rad),
centerY-maxRange*Math.sin(rad)
);

ctx.stroke();

}

function drawObject(){

if(iDistance>40 || iDistance<=0) return;

let pixDist=iDistance*((canvas.height-canvas.height*0.1666)*0.025);

let rad=iAngle*Math.PI/180;

ctx.strokeStyle="rgb(255,10,10)";
ctx.lineWidth=9;

ctx.beginPath();

ctx.moveTo(
pixDist*Math.cos(rad)+centerX,
-pixDist*Math.sin(rad)+centerY
);

ctx.lineTo(
maxRange*Math.cos(rad)+centerX,
-maxRange*Math.sin(rad)+centerY
);

ctx.stroke();

}

function drawText(){

ctx.fillStyle="black";
ctx.fillRect(0,canvas.height*0.935,canvas.width,canvas.height);

ctx.fillStyle="rgb(98,245,31)";
ctx.font="25px Arial";

/* Distance scale */
ctx.fillText("10cm",canvas.width*0.62,canvas.height*0.91);
ctx.fillText("20cm",canvas.width*0.72,canvas.height*0.91);
ctx.fillText("30cm",canvas.width*0.82,canvas.height*0.91);
ctx.fillText("40cm",canvas.width*0.92,canvas.height*0.91);

ctx.font="40px Arial";

ctx.fillText("RADAR SYSTEM",canvas.width*0.02,canvas.height*0.97);
ctx.fillText("Angle: "+iAngle,canvas.width*0.45,canvas.height*0.97);

if(iDistance<20 && iDistance>0)
ctx.fillStyle="red";

ctx.fillText("Distance: "+iDistance+" cm",canvas.width*0.7,canvas.height*0.97);

if(iDistance<20 && iDistance>0){

ctx.fillStyle="red";
ctx.font="80px Arial";

ctx.fillText("WARNING: PROXIMITY",canvas.width/2-350,canvas.height/2);

}

}

function updateRadar(){

fetch("/data")

.then(r=>r.text())

.then(txt=>{

let d=txt.split(",");

iAngle=parseInt(d[0]);
iDistance=parseInt(d[1]);

fadeBackground();
drawRadar();
drawLine();
drawObject();
drawText();

});

}

setInterval(updateRadar,40);

</script>

</body>
</html>

)rawliteral";

/* ===================== ULTRASONIC ===================== */

long getDistance(){

digitalWrite(TRIG,LOW);
delayMicroseconds(2);

digitalWrite(TRIG,HIGH);
delayMicroseconds(10);
digitalWrite(TRIG,LOW);

long duration=pulseIn(ECHO,HIGH,30000);

if(duration==0) return 0;

return duration*0.034/2;

}

/* ===================== WEB HANDLERS ===================== */

void handleRoot(){

server.send_P(200,"text/html",webpage);

}

void handleData(){

server.send(200,"text/plain",String(angle)+","+String(distanceCM));

}

/* ===================== SETUP ===================== */

void setup(){

pinMode(TRIG,OUTPUT);
pinMode(ECHO,INPUT);
pinMode(BUZZER,OUTPUT);
pinMode(LED,OUTPUT);

radarServo.attach(SERVO_PIN);

WiFi.softAP("ESP_Radar","12345678");

server.on("/",handleRoot);
server.on("/data",handleData);

server.begin();

}

/* ===================== LOOP ===================== */

void loop(){

radarServo.write(angle);

distanceCM=getDistance();

/* ALERT SYSTEM */

if(distanceCM>0 && distanceCM<20){

digitalWrite(BUZZER,HIGH);
digitalWrite(LED,HIGH);

}
else{

digitalWrite(BUZZER,LOW);
digitalWrite(LED,LOW);

}

angle+=dir;

if(angle>=180 || angle<=0)
dir=-dir;

server.handleClient();

delay(40);

}