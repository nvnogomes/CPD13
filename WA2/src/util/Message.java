package util;

import java.net.*;

public class Message {

	
	public static void send(DatagramPacket packet, String rsp) {
		try {
			DatagramSocket dSocket = new DatagramSocket(packet.getSocketAddress());
			
			byte[] buf = rsp.getBytes();
			DatagramPacket resp = new DatagramPacket(buf, buf.length);
			dSocket.send(resp);
			
			dSocket.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
}
