package util;

import java.net.*;

public class Message {

	private static final int TIMEOUT = 1000;
        
    public static byte[] send(SocketAddress sa, String msg, boolean ret) {
    	
    	byte[] returnValue = null;
        try {
            DatagramSocket dSocket = new DatagramSocket();
                
            byte[] buf = msg.getBytes();
            DatagramPacket resp = new DatagramPacket(buf, buf.length,sa);
            dSocket.send(resp);
            
            if( ret ) {
            	byte[] buffer = new byte[2048];
    			DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
            	dSocket.setSoTimeout(TIMEOUT);
            	dSocket.receive(packet);
            	returnValue = buffer;
            }
            
            dSocket.close();
            
        } catch(SocketTimeoutException e) {
        } catch (Exception e) {
        	e.printStackTrace();
        }
        
        return returnValue;
    }
    
    
    public static byte[] sendOperation(SocketAddress sa, String rsp) {
    	return send(sa, rsp, true);
    }
    
    public static void sendResponse(SocketAddress sa, String rsp) {
    	send(sa, rsp, false);
    }
       
}