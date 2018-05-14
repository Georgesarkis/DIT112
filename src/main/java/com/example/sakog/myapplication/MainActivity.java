package com.example.sakog.myapplication;

import android.annotation.SuppressLint;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.speech.SpeechRecognizer;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageButton;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.util.UUID;


public class MainActivity extends AppCompatActivity {
    //BLUETOOTH
    String address = null;
    private ProgressDialog progress;
    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket = null;
    private boolean isBtConnected = false;
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    // handels the input from the arduino
    Handler handler;
    private StringBuilder sb = new StringBuilder();
    // Status  for Handler
    final int RECIEVE_MESSAGE = 1;




    //VOICE RECOGNITION
    /*private SpeechRecognizer mSpeechRocognizer;
    private static final String TAG = MainActivity.class.getName();
    private Handler mHandler = new Handler();
    Intent mSpeechIntent;
    private static final String[] VALID_COMMANDS = {
            "fast","default","slow","park","turn left","turn right", "forward", "backward","stop"
    };
    private static final int VALID_COMMAND_SIZE = VALID_COMMANDS.length;
    */
    //private boolean parkingStatus = false;

    //BUTTONS
    TextView textView;
    ImageButton backward ;
    ImageButton forward ;
    ImageButton right ;
    ImageButton left ;
    ImageButton volt ;
    ImageButton parkingMode ;
    ImageButton camera;
    SeekBar seekbar ;
    String command;

    @SuppressLint({"ClickableViewAccessibility", "HandlerLeak"})
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        textView = findViewById(R.id.textView);
        volt = findViewById(R.id.volt);
        backward = findViewById(R.id.down);
        forward = findViewById(R.id.forward);
        right = findViewById(R.id.rightTurn);
        left = findViewById(R.id.leftTurn);
        parkingMode = findViewById(R.id.parkingMode);
        seekbar =  findViewById(R.id.seekBar);
        camera = findViewById(R.id.camera);

        Intent newInt = getIntent();
        address = newInt.getStringExtra(bluetooth_list.EXTRA_ADDRESS); //receive the address of the bluetooth device
        new ConnectBT().execute(); //Call the class to connect

        //seekbar the number changes from 0 to 100
        seekbar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            int progressChangedValue = 0;

            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                progressChangedValue = progress;
            }

            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            public void onStopTrackingTouch(SeekBar seekBar) {
                textView.setText(Integer.toString(progressChangedValue));
                if(progressChangedValue < 25){
                    sendMessage("1"); //set speed for the car on 15
                }
                else if (progressChangedValue < 50 ){
                    sendMessage("2"); //set speed for the car on 30
                }
                else if (progressChangedValue < 75 ){
                    sendMessage("3"); //set speed for the car on 45
                }
                else{
                    sendMessage("4"); //set speed for the car on 60
                }
            }
        });

        //TODO first one to try if didn't work try TODO2
        handler = new Handler() {
            public void handleMessage(android.os.Message msg) {
                switch (msg.what) {
                    case RECIEVE_MESSAGE:                                                   // if receive massage
                        byte[] readBuf = (byte[]) msg.obj;
                        String strIncom = new String(readBuf, 0, msg.arg1);                 // create string from bytes array
                        sb.append(strIncom);                                                // append string
                        int endOfLineIndex = sb.indexOf("\r\n");                            // determine the end-of-line
                        if (endOfLineIndex > 0) {                                            // if end-of-line,
                            String sbprint = sb.substring(0, endOfLineIndex);               // extract string
                            sb.delete(0, sb.length());                                      // and clear
                            Toast.makeText(getApplicationContext(), "The voltage is: " + sbprint, Toast.LENGTH_LONG).show();
                        }
                        //Log.d(TAG, "...String:"+ sb.toString() +  "Byte:" + msg.arg1 + "...");
                        break;
                }
            };
        };
        //On touch listener for the voice contral
        volt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                command = "v";
                sendMessage(command);
                //TODO this is the TODO2 look for the referencees
                //receiveData();

            }
        });

        //On touch listener for the backwarding button
        backward.setOnTouchListener(new View.OnTouchListener(){
            @Override
            public boolean onTouch(View view,MotionEvent event){
                if (event.getAction() == MotionEvent.ACTION_DOWN) { //MotionEvent.ACTION_DOWN is when you hold a button down
                    command = "b";
                    sendMessage(command);
                    }
                else if (event.getAction() == MotionEvent.ACTION_UP) { //MotionEvent.ACTION_UP is when you release a button
                    command = "s";
                    sendMessage(command);
                    }
                return false;
            }
        });
        //On touch listener for the forward button
        forward.setOnTouchListener(new View.OnTouchListener(){
            @Override
            public boolean onTouch(View view,MotionEvent event){
                if (event.getAction() == MotionEvent.ACTION_DOWN) { //MotionEvent.ACTION_DOWN is when you hold a button down
                    command = "f";
                    sendMessage(command);
                    }
                else if(event.getAction() == MotionEvent.ACTION_UP){ //MotionEvent.ACTION_UP is when you release a button
                    command = "s";
                    sendMessage(command);
                    }

                return false;
            }
        });
        //On touch listener for the turn left button
        left.setOnTouchListener(new View.OnTouchListener(){
            @Override
            public boolean onTouch(View view,MotionEvent event){
                if (event.getAction() == MotionEvent.ACTION_DOWN){ //MotionEvent.ACTION_DOWN is when you hold a button down
                    command = "l";
                    sendMessage(command);
                }
                else if(event.getAction() == MotionEvent.ACTION_UP){ //MotionEvent.ACTION_UP is when you release a button
                    command = "s";
                    sendMessage(command);
                }
                return false;
            }
        });
        //On touch listener for the turn right button
        right.setOnTouchListener(new View.OnTouchListener(){
            @Override
            public boolean onTouch(View view,MotionEvent event){
                if (event.getAction() == MotionEvent.ACTION_DOWN){ //MotionEvent.ACTION_DOWN is when you hold a button down
                    command = "r";
                    sendMessage(command);
                }
                else if(event.getAction() == MotionEvent.ACTION_UP){ //MotionEvent.ACTION_UP is when you release a button
                    command = "S";
                    sendMessage(command);
                }
                return false;
            }
        });
        parkingMode.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //if (!parkingStatus) {
                    command = "p";
                //  parkingMode.setImageResource(R.drawable.stop);
                //  parkingStatus = true;
                    sendMessage(command);
                //}
                /*else {
                    command = "m";
                    // change the logo when pressed
                    parkingMode.setImageResource(R.drawable.parking);
                    parkingStatus = false;
                    sendMessage(command);
                }*/
            }
        });
        camera.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent startIntent = new Intent(getApplicationContext(),camera_view.class);

                //TO pass information to second screen
                camera_view.getSocket(btSocket);
                startActivity(startIntent);
            }
        });



    }


    private void sendMessage(String rMsg){
        if (btSocket!=null) {
            try {
                btSocket.getOutputStream().write(rMsg.getBytes());
            } catch (IOException e) {
                Toast.makeText(getApplicationContext(), "Error Sending Message!", Toast.LENGTH_LONG).show();
            }
        }
    }
    //TODO this is the old one didn't work
    /*private int readVolt(){
        int volt ;
        try {
           volt = btSocket.getInputStream().read();
        } catch (IOException e) {
            volt = 99999999;
        }
        return volt;
    }*/
    //TODO this is the TODO2
    /*public void receiveData() {
        byte[] buffer = new byte[1024];
        int bytes;

        while (true) {
            try {
                bytes = btSocket.getInputStream().read(buffer);

                String strReceived = new String(buffer, 0, bytes);
                Toast.makeText(getApplicationContext(), "The voltage is: " + strReceived, Toast.LENGTH_LONG).show();
            } catch (IOException e) {
                Toast.makeText(getApplicationContext(), "Could not read the inputs" , Toast.LENGTH_LONG).show();
            }
        }
    }*/

    private class ConnectBT extends AsyncTask<Void, Void, Void>  // UI thread
    {
        private boolean ConnectSuccess = true; //if it's here, it's almost connected

        @Override
        protected void onPreExecute() {
            progress = ProgressDialog.show(MainActivity.this, "Connecting...", "Please wait!!!");  //show a progress dialog
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
    }
}
