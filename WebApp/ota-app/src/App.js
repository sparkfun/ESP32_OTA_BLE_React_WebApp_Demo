import React from 'react';
import Popup from 'react-popup';
import './App.css';

var myESP32 = 'e804b643-6ce7-4e81-9f8a-ce0f699085eb'

var otaServiceUuid = 'b8659210-af91-4ad3-a995-a58d6fd26145'
var versionCharacteristicUuid = 'b8659212-af91-4ad3-a995-a58d6fd26145'
var fileCharacteristicUuid = 'b8659211-af91-4ad3-a995-a58d6fd26145'

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
var latestSoftware = "N/A";

const characteristicSize = 512;

/* BTConnect
 * Brings up the bluetooth connection window and filters for the esp32
 * TODO:
 * 10-Add handler for disconnect so page doesn't crash
 */
function BTConnect(){
  navigator.bluetooth.requestDevice({
    filters: [{
      services: [myESP32]
    }],
    optionalServices: [otaServiceUuid]
  })
  .then(device => device.gatt.connect())
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
  })
  //Grab our version numbers from Github
  .then(_ => fetch('https://raw.githubusercontent.com/sparkfun/ESP32_OTA_BLE_React_WebApp_Demo/master/version.json?token=AHAFCBQ5IDR5PW7K3TJO2I267TK2E'))
  .then(function (response) {
    // The API call was successful!
    return response.json();
  })
  .then(function (data) {
    // JSON should be formatted so that 0'th entry is the newest version
    latestSoftware = data.firmware[0]['software'];
    console.log(latestSoftware);
    document.getElementById('hw_version').innerHTML = "Hardware: " + currentHardwareVersion;
    document.getElementById('sw_version').innerHTML = "Software: " + softwareVersion;
    if (latestSoftware === softwareVersion)
    {
      //Software is updated, do nothing.
    }
    else {
      var compatibleHardwareVersion = "N/A"
      var hardwareNumber = 0;
      while (compatibleHardwareVersion !== undefined) {
        compatibleHardwareVersion = data.firmware[0]['hardware'][hardwareNumber++];
        if (compatibleHardwareVersion === currentHardwareVersion)
        {
          console.log(currentHardwareVersion);
        }
        PromptUserForUpdate()
      }
    }
  })
  .catch(error => { console.log(error); });
}

/* PromptUserForUpdate()
 * Downloads the firmware based on the hardware version and latest software version
 * TODO:
 * 7-Make separate public repo for firmware and patchnotes
 * 5-Make github folder structure for different hardware versions
 * 6-Look at the specific release that matches software we want to switch to (Have html adapt to changing HW and SW versions)
 * 4-Get patchnotes from github release page(???) and display them with update prompt
 * 6-Use CSS to turn Popup into actual popup
 */
function PromptUserForUpdate(){
  fetch('https://raw.githubusercontent.com/AndyEngland521/Tet/master/Firmware/main/compiled/v1_3.bin')
  .then(function (response) {
    return response.arrayBuffer();
  })
  .then(function (data) {
    Popup.create({
      content: "Version " + softwareVersion + " is out of date. Update to " + latestSoftware + "?",
      buttons: {
          left: [{
              text: 'Yes',
              action: function () {
                Popup.close();
                return SendFileOverBluetooth(data);
              }
          }],
          right: [{
              text: 'No',
              action: function () {
                /** Close this popup. Close will always close the current visible one, if one is visible */
                Popup.close();
              }
          }]
      }
  })
  })
  .catch(function (err) { console.warn('Something went wrong.', err); });
}

/* SendFileOverBluetooth(data)
 * Figures out how large our update binary is, attaches an eventListener to our dataCharacteristic so the Server can tell us when it has finished writing the data to memory
 * Calls sendBufferedData(), which begins a loop of write, wait for ready flag, write, wait for ready flag...
 * TODO:
 * 8-Disconnect the eventListener once the update is complete
 * 7-Call sendBufferedData from promise returned by addEventListener or startNotifications (???Slight reliability improvement???)
 */
function SendFileOverBluetooth(data) {
  if(!esp32Service)
  {
    console.log("No esp32 Service");
    return;
  }

  updateData = data;
  totalSize = updateData.byteLength;
  remaining = totalSize;
  amountToWrite = 0;
  currentPosition = 0;

  esp32Service.getCharacteristic(fileCharacteristicUuid)
  .then(characteristic => {
    readyFlagCharacteristic = characteristic;
    return characteristic.startNotifications()
    .then(_ => {
      readyFlagCharacteristic.addEventListener('characteristicvaluechanged', sendBufferedData)
    });
  })
  .catch(error => { 
    console.log(error); 
  });
  sendBufferedData();
}


/* SendBufferedData()
 * An ISR attached to the same characteristic that it writes to, this function slices data into characteristic sized chunks and sends them to the Server
 */
function sendBufferedData() {
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
      .then(characteristic => recursiveSend(characteristic, dataToSend))
      .catch(error => { 
        console.log(error); 
      });
  }
}


/* resursiveSend()
 * Returns a promise to itself to ensure data was sent and the promise is resolved.
 */
function recursiveSend(characteristic, data) {
  return characteristic.writeValue(data)
  .catch(error => {
    return recursiveSend(characteristic, data);
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
    <button id="connect"
    onClick={BTConnect}
    >
      Connect to Bluetooth
    </button>
    <button id="Update Firmware"
    onClick={CheckVersion}
    >
      CheckVersion
    </button>
  </header>
</div>
)
}

export default App;