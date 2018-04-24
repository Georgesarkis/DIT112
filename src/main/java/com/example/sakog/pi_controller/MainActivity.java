package com.example.sakog.pi_controller;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.SeekBar;
import android.widget.TextView;

import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

public class MainActivity extends AppCompatActivity {

    Client client;
    //Socket myAppSocket = null;
    //buttons
    ImageButton camera;
    ImageButton parking;
    ImageButton voiceControl;
    ImageButton forward;
    ImageButton backward;
    ImageButton turnLeft;
    ImageButton turnRight;
    SeekBar seekbar;
    TextView textView;
    int command;
    public static String wifiModuleIp = "";
    public static int wifiModulePort = 0;
    Socket_AsyncTask cmd_increase_servo;
    //command to send to the PI


    @SuppressLint("ClickableViewAccessibility")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        //client.execute();
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        camera = findViewById(R.id.camera);
        parking = findViewById(R.id.parking);
        voiceControl = findViewById(R.id.microphone);
        forward = findViewById(R.id.forward);
        backward = findViewById(R.id.backward);
        turnLeft = findViewById(R.id.turnLeft);
        turnRight = findViewById(R.id.turnRight);
        seekbar = findViewById(R.id.seekBar);
        textView = findViewById(R.id.textView);


        //create the port which pi and andorid will connect
        //getIPandPort();
        //create the socket sync
        cmd_increase_servo = new Socket_AsyncTask();


        voiceControl.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View arg0){
                client= new Client("192.168.0.101", 8080);
                client.execute();
                /*if (event.getAction() == MotionEvent.ACTION_DOWN) { //MotionEvent.ACTION_DOWN is when you hold a button down
                    command = 102;client.sendCommand(command);
                    //    cmd_increase_servo.execute();
                }
                else if (event.getAction() == MotionEvent.ACTION_UP) { //MotionEvent.ACTION_UP is when you release a button
                    command = 0;client.sendCommand(command);
                    //    cmd_increase_servo.execute();

                }*/
                //return false;
            }
        });
        //to go to the camera mode
        camera.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
               Intent startIntent = new Intent(getApplicationContext(),CameraScreen.class);
                //TO pass information to second screen
               startActivity(startIntent);
            }
        });

        //to change the speed of the car
        seekbar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            int progressChangedValue = 0;

            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                progressChangedValue = progress;
            }

            public void onStartTrackingTouch(SeekBar seekBar) {
                // TODO Auto-generated method stub
            }

            public void onStopTrackingTouch(SeekBar seekBar) {
                textView.setText("Speed: " + Integer.toString(progressChangedValue));
                command = progressChangedValue;
                //cmd_increase_servo.execute();
                //client.sendCommand(command);
            }
        });

        //going forward
        forward.setOnTouchListener(new View.OnTouchListener(){
            @Override
            public boolean onTouch(View view,MotionEvent event){
                if (event.getAction() == MotionEvent.ACTION_DOWN) { //MotionEvent.ACTION_DOWN is when you hold a button down
                    command = 102;client.sendCommand(command);
                   // cmd_increase_servo.execute();
                }
                else if (event.getAction() == MotionEvent.ACTION_UP) { //MotionEvent.ACTION_UP is when you release a button
                    command = 0;client.sendCommand(command);
                  //  cmd_increase_servo.execute();

                }
                return false;
            }
        });

        //going backward
        backward.setOnTouchListener(new View.OnTouchListener(){
            @Override
            public boolean onTouch(View view,MotionEvent event){
                if (event.getAction() == MotionEvent.ACTION_DOWN) { //MotionEvent.ACTION_DOWN is when you hold a button down
                 //   command = 102;client.sendCommand(command);
                 //   cmd_increase_servo.execute();
                }
                else if (event.getAction() == MotionEvent.ACTION_UP) { //MotionEvent.ACTION_UP is when you release a button
                 //   command = 0;client.sendCommand(command);
                 //   cmd_increase_servo.execute();

                }
                return false;
            }
        });

        //to start the parking mode
        parking.setOnTouchListener(new View.OnTouchListener(){
            @Override
            public boolean onTouch(View view,MotionEvent event){
                if (event.getAction() == MotionEvent.ACTION_DOWN) { //MotionEvent.ACTION_DOWN is when you hold a button down
                //    command = 102;client.sendCommand(command);
                //    cmd_increase_servo.execute();
                }
            //    else if (event.getAction() == MotionEvent.ACTION_UP) { //MotionEvent.ACTION_UP is when you release a button when inters the parking mode should not exit until it's parked
            //        command = 105;
            //        sendResponse(command); TODO
            //    }

                return false;
            }
        });

        //to turn left
        turnLeft.setOnTouchListener(new View.OnTouchListener(){
            @Override
            public boolean onTouch(View view,MotionEvent event){
                if (event.getAction() == MotionEvent.ACTION_DOWN) { //MotionEvent.ACTION_DOWN is when you hold a button down
                //    command = 102;client.sendCommand(command);
                //    cmd_increase_servo.execute();
                }
                else if (event.getAction() == MotionEvent.ACTION_UP) { //MotionEvent.ACTION_UP is when you release a button
                //    command = 0;client.sendCommand(command);
                //    cmd_increase_servo.execute();

                }
                return false;
            }
        });

        //to turn right
        turnRight.setOnTouchListener(new View.OnTouchListener(){
            @Override
            public boolean onTouch(View view,MotionEvent event){
                if (event.getAction() == MotionEvent.ACTION_DOWN) { //MotionEvent.ACTION_DOWN is when you hold a button down
                //    command = 102;client.sendCommand(command);
                //    cmd_increase_servo.execute();
                }
                else if (event.getAction() == MotionEvent.ACTION_UP) { //MotionEvent.ACTION_UP is when you release a button
                //    command = 0;client.sendCommand(command);
                //    cmd_increase_servo.execute();

                }
                return false;
            }
        });
    }

/*    public void getIPandPort()
    {
        String iPandPort = "";
        Log.d("MYTEST","IP String: "+ iPandPort);
        String temp[]= iPandPort.split(":");
        wifiModuleIp = temp[0];
        wifiModulePort = Integer.valueOf(temp[1]);
        Log.d("MY TEST","IP:" +wifiModuleIp);
        Log.d("MY TEST","PORT:"+wifiModulePort);
    }*/
    public class Socket_AsyncTask extends AsyncTask<Void,Void,Void>
    {
        Socket socket;
        String address = "192.168.0.101"; //pi address
        int port = 8080; // to accses it you should write "java host" in terminal
        @Override
        protected Void doInBackground(Void... params){
            try{
                //InetAddress inetAddress = InetAddress.getByName("192.168.0.168");
                socket = new Socket(address, port);
                DataOutputStream dataOutputStream = new DataOutputStream(socket.getOutputStream());
                dataOutputStream.writeBytes(String.valueOf(command));
                dataOutputStream.close();
                socket.close();
            }catch (UnknownHostException e){
                e.printStackTrace();
            }
            catch (IOException e){
                e.printStackTrace();
            }
            return null;
        }
    }



}
