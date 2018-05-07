package com.example.sakog.myapplication;

import android.app.Activity;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Toast;


import java.io.IOException;
import java.util.UUID;


public class camera_view extends Activity {

    SensorManager sensorManager;
    Sensor sensor;

    String address = null;
    private ProgressDialog progress;
    BluetoothAdapter myBluetooth = null;
    static BluetoothSocket btSocket = null;
    private boolean isBtConnected = false;
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private WebView webView;

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //sensor reading
        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        sensor = sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE_UNCALIBRATED);


        // creating the web page and reading the url
        webView = findViewById(R.id.webView1);
        webView.getSettings().setJavaScriptEnabled(true);
        webView.loadUrl("http://www.google.com");

        //to make it run the page in the app not in default browser
        webView.setWebViewClient(new WebViewClient(){
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url){
                view.loadUrl(url);
                return true;
            }
        });

    }


    //gyroscop readings and passing it to the arduino
    public void onResume() {
        super.onResume();
        sensorManager.registerListener(gyroListener, sensor,
                SensorManager.SENSOR_DELAY_NORMAL);
    }

    public void onStop() {
        super.onStop();
        sensorManager.unregisterListener(gyroListener);
    }

    public SensorEventListener gyroListener = new SensorEventListener() {
        public void onAccuracyChanged(Sensor sensor, int acc) { }

        public void onSensorChanged(SensorEvent event) {
            float x = event.values[0];
            float y = event.values[1];
            float z = event.values[2];
            x = (int)x;
            y = (int)y;
            z = (int)z;
            //on portrait mode the +x goes to backward -x goes to forward
            //+z goes to left - z goes to right

            //on landscape mode the +y goes forward -y goes backward
            //+z goes left -z goes right

            //we are going to use in portrait mode
            String direction = "stop";
            if(x > 0){
                if(direction.equals("forward")){
                    sendMessage("s");
                    direction = "stop";
                }
                else{
                    sendMessage("b");
                    direction = "backward";
                }
            }
            if(x < 0){
                if(direction.equals("backward")){
                    sendMessage("s");
                    direction = "stop";
                }
                else {
                    sendMessage("f");
                    direction = "forward";
                }
            }
            if(z > 0){
                if(direction.equals("right")){
                    sendMessage("s");
                    direction = "stop";
                }
                else{
                    sendMessage("l");
                    direction = "left";
                }
            }
            if(z < 0){
                if(direction.equals("left")){
                    sendMessage("s");
                    direction = "stop";
                }
                else{
                    sendMessage("r");
                    direction = "right";
                }
            }
        }
    };
    //bluetooth connection
    public static void getSocket(BluetoothSocket Socket){
        btSocket = Socket;
    }
    //send input to arduino
    private void sendMessage(String rMsg){
        if (btSocket!=null) {
            try {
                btSocket.getOutputStream().write(rMsg.getBytes());
            } catch (IOException e) {
                Toast.makeText(getApplicationContext(), "Error Sending Message!", Toast.LENGTH_LONG).show();
            }
        }
    }

    /* reconnect to bluetooth in case it is needed
    private class ConnectBT extends AsyncTask<Void, Void, Void>  // UI thread
    {
        private boolean ConnectSuccess = true; //if it's here, it's almost connected

        @Override
        protected void onPreExecute() {
            progress = ProgressDialog.show(camera_view.this, "Connecting...", "Please wait!!!");  //show a progress dialog
        }

        @Override
        protected Void doInBackground(Void... devices){ //while the progress dialog is shown, the connection is done in background
            try {
                if (btSocket == null || !isBtConnected) {
                    myBluetooth = BluetoothAdapter.getDefaultAdapter();//get the mobile bluetooth device
                    BluetoothDevice dispositivo = myBluetooth.getRemoteDevice(address);//connects to the device's address and checks if it's available
                    btSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID);//create a RFCOMM (SPP) connection
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();//start connection
                }
            } catch (IOException e) {
                ConnectSuccess = false;//if the try failed, you can check the exception here
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void result){ //after the doInBackground, it checks if everything went fine
            super.onPostExecute(result);

            if (!ConnectSuccess) {
                msg("Connection Failed. Is it a SPP Bluetooth? Try again.");
                finish();
            } else {
                msg("Connected.");
                isBtConnected = true;
            }
            progress.dismiss();
        }

        // fast way to call Toast
        private void msg(String s) {
            Toast.makeText(getApplicationContext(),s,Toast.LENGTH_LONG).show();
        }
    }*/
}