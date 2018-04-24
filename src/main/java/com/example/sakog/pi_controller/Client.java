package com.example.sakog.pi_controller;

import android.os.AsyncTask;
import android.widget.TextView;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;
import java.net.UnknownHostException;

public class Client extends AsyncTask<Void, Void, Void> {

	String dstAddress;
	int dstPort;
	String response = "";
	Socket socket = null;
	DataOutputStream dos;

	Client(String addr, int port) {
		dstAddress = addr;
		dstPort = port;
	}

	@Override
	protected Void doInBackground(Void... arg0) {
		try {
			socket = new Socket(dstAddress, dstPort);
			dos = new DataOutputStream(socket.getOutputStream());


			ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream(
					1024);
			byte[] buffer = new byte[1024];

			int bytesRead;
			InputStream inputStream = socket.getInputStream();


			//  notice: inputStream.read() will block if no data return

			while ((bytesRead = inputStream.read(buffer)) != -1) {
				byteArrayOutputStream.write(buffer, 0, bytesRead);
				response += byteArrayOutputStream.toString("UTF-8");
			}

		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			response = "UnknownHostException: " + e.toString();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			response = "IOException: " + e.toString();
		} finally {
			if (socket != null) {
				try {
					socket.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		return null;
	}
	public void sendCommand(int command){
		try {
			dos.writeInt(command);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	public void closeSocket(){
		try {
			dos.close();
			socket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

	}

	@Override
	protected void onPostExecute(Void result) {
		//textResponse.setText(response);
		super.onPostExecute(result);
	}

}
