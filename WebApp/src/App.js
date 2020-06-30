  
/*************************************************** 
 This is a React WebApp written to Flash an ESP32 via BLE
 
 Written by Andrew England (SparkFun)
 BSD license, all text above must be included in any redistribution.
 *****************************************************/
 
 import React from 'react';
import Popup from 'react-popup';
import './App.css';

var myESP32 = 'd804b643-6ce7-4e81-9f8a-ce0f699085eb'

var otaServiceUuid = 'c8659210-af91-4ad3-a995-a58d6fd26145'
var versionCharacteristicUuid = 'c8659212-af91-4ad3-a995-a58d6fd26145'
var fileCharacteristicUuid = 'c8659211-af91-4ad3-a995-a58d6fd26145'

let esp32Device = null;
let esp32Service = null;
let readyFlagCharacteristic = null;
let dataToSend = null;
let updateData = null;

var totalSize;
var remaining;
var amountToWrite;
var currentPosition;

var currentHardwareVersion = "N/A";
var softwareVersion = "N/A";
var latestCompatibleSoftware = "N/A";

const characteristicSize = 512;

/* BTConnect
 * Brings up the bluetooth connection window and filters for the esp32
 */
function BTConnect(){
  navigator.bluetooth.requestDevice({
    filters: [{
      services: [myESP32]
    }],
    optionalServices: [otaServiceUuid]
  })
  .then(device => {
    return device.gatt.connect()
  })
  .then(server => server.getPrimaryService(otaServiceUuid))
  .then(service => {
    esp32Service = service;
  })
  .then(service => {
    return service;
  })
  .then(_ => {
    return CheckVersion();
  })
  .catch(error => { console.log(error); });
}

/* onDisconnected(event)
 * If the device becomes disconnected, prompt the user to reconnect.
 */
function onDisconnected(event) {
  Popup.create({
    content: esp32Device.name + ' is disconnected, would you like to reconnect?',
    buttons: {
      left: [{
          text: 'Yes',
          action: function () {
            Popup.close();
            BTConnect();
          }
      }],
      right: [{
          text: 'No',
          action: function () {
            Popup.close();
          }
      }]
    }
  })
}

/* CheckVersion()
 * Grab most current version from Github and Server, if they don't match, prompt the user for firmware update
 */
function CheckVersion(){
  if(!esp32Service)
  {
    return;
  }
  return esp32Service.getCharacteristic(versionCharacteristicUuid)
  .then(characteristic => characteristic.readValue())
  .then(value => {
    currentHardwareVersion = 'v' + value.getUint8(0) + '.' + value.getUint8(1);
    softwareVersion = 'v' + value.getUint8(2) + '.' + value.getUint8(3) + '.' + value.getUint8(4);
    document.getElementById('hw_version').innerHTML = "Hardware: " + currentHardwareVersion;
    document.getElementById('sw_version').innerHTML = "Software: " + softwareVersion;
  })
  //Grab our version numbers from Github
  .then(_ => fetch('https://raw.githubusercontent.com/sparkfun/ESP32_OTA_BLE_React_WebApp_Demo/master/GithubRepo/version.json'))
  .then(function (response) {
    // The API call was successful!
    return response.json();
  })
  .then(function (data) {
    // JSON should be formatted so that 0'th entry is the newest version
    if (latestCompatibleSoftware === softwareVersion)
    {
      //Software is updated, do nothing.
    }
    else {
      var softwareVersionCount = 0;
      latestCompatibleSoftware = data.firmware[softwareVersionCount]['software'];
      versionFindLoop:
        while (latestCompatibleSoftware !== undefined) {
          var compatibleHardwareVersion = "N/A"
          var hardwareVersionCount = 0;
          while (compatibleHardwareVersion !== undefined) {
            compatibleHardwareVersion = data.firmware[softwareVersionCount]['hardware'][hardwareVersionCount++];
            if (compatibleHardwareVersion === currentHardwareVersion)
            {
              latestCompatibleSoftware = data.firmware[softwareVersionCount]['software'];
              if (latestCompatibleSoftware !== softwareVersion)
              {
                console.log(latestCompatibleSoftware);
                PromptUserForUpdate();
              }
              break versionFindLoop;
            }
          }
          softwareVersionCount++;
        }
      }
  })
  .catch(error => { console.log(error); });
}

/* PromptUserForUpdate()
 * Asks the user if they want to update, if yes downloads the firmware based on the hardware version and latest software version and begins sending
 */
function PromptUserForUpdate(){
    Popup.create({
      content: "Version " + softwareVersion + " is out of date. Update to " + latestCompatibleSoftware + "?",
      buttons: {
          left: [{
              text: 'Yes',
              action: function () {
                fetch('https://raw.githubusercontent.com/sparkfun/ESP32_OTA_BLE_React_WebApp_Demo/' + latestCompatibleSoftware + '/GithubRepo/' + currentHardwareVersion + '.bin')
                .then(function (response) {
                  return response.arrayBuffer();
                })
                .then(function (data) {
                  Popup.close();
                  
                  updateData = data;
                  return SendFileOverBluetooth();
                })
                .catch(function (err) { console.warn('Something went wrong.', err); });
              }
          }],
          right: [{
              text: 'No',
              action: function () {
                Popup.close();
              }
          }]
      }
  })
}

/* SendFileOverBluetooth(data)
 * Figures out how large our update binary is, attaches an eventListener to our dataCharacteristic so the Server can tell us when it has finished writing the data to memory
 * Calls SendBufferedData(), which begins a loop of write, wait for ready flag, write, wait for ready flag...
 */
function SendFileOverBluetooth() {
  if(!esp32Service)
  {
    console.log("No esp32 Service");
    return;
  }

  totalSize = updateData.byteLength;
  remaining = totalSize;
  amountToWrite = 0;
  currentPosition = 0;
  esp32Service.getCharacteristic(fileCharacteristicUuid)
  .then(characteristic => {
    readyFlagCharacteristic = characteristic;
    return characteristic.startNotifications()
    .then(_ => {
      readyFlagCharacteristic.addEventListener('characteristicvaluechanged', SendBufferedData)
    });
  })
  .catch(error => { 
    console.log(error); 
  });
  SendBufferedData();
}


/* SendBufferedData()
 * An ISR attached to the same characteristic that it writes to, this function slices data into characteristic sized chunks and sends them to the Server
 */
function SendBufferedData() {
  if (remaining > 0) {
    if (remaining >= characteristicSize) {
      amountToWrite = characteristicSize
    }
    else {
      amountToWrite = remaining;
    }
    dataToSend = updateData.slice(currentPosition, currentPosition + amountToWrite);
    currentPosition += amountToWrite;
    remaining -= amountToWrite;
    console.log("remaining: " + remaining);
    esp32Service.getCharacteristic(fileCharacteristicUuid)
      .then(characteristic => RecursiveSend(characteristic, dataToSend))
      .then(_ => {
        return document.getElementById('completion').innerHTML = (100 * (currentPosition/totalSize)).toPrecision(3) + '%';
      })
      .catch(error => { 
        console.log(error); 
      });
  }
}


/* resursiveSend()
 * Returns a promise to itself to ensure data was sent and the promise is resolved.
 */
function RecursiveSend(characteristic, data) {
  return characteristic.writeValue(data)
  .catch(error => {
    return RecursiveSend(characteristic, data);
  });
}


/* App()
 * The meat and potatoes of our web-app; where it all comes together
 */
function App() {
return (
<div className="App" id="top">
  <header className="App-header" id="mid">
    <Popup />
    <p id="hw_version">Hardware: Not Connected</p>
    <p id="sw_version">Software: Not Connected</p>
    <p id="completion"></p>
    <button id="connect"
    onClick={BTConnect}
    >
      Connect to Bluetooth
    </button>
  </header>
</div>
)
}

export default App;