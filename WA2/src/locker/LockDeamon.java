package locker;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;


import util.Converter;
import util.LockMessage;
import util.Message;
import util.Status;


public class LockDeamon extends Thread {

	/**
	 * Deamon port.
	 */
	public final static int PORT = 46666;
	
	/**
	 * Datagram socket.
	 */
	private DatagramSocket dSocket;
	
	/**
	 * 
	 */
	private LockServer locker;
	
	
	
	public LockDeamon() {
		locker = new LockServer();
	}
	
	public void startDeamon() throws SocketException {
		dSocket = new DatagramSocket(PORT);
		start();
	}
	
	/**
	 * Thread code
	 */
	public void run() {
		
		while ( true ) {
			byte[] buffer = new byte[2048];
			DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
			LockMessage lMsg = new LockMessage();
			Status ret;
			try {
				dSocket.receive(packet);
				lMsg.build( Converter.byteArrayToString(buffer) );
				switch(lMsg.getOp() ) {
				case LREAD:
					ret = locker.lockRead(lMsg.getAccountId());
					Message.sendResponse(packet.getSocketAddress(), ret.toString());
					break;
				case LWRITE:
					ret = locker.lockWrite(lMsg.getAccountId());
					Message.sendResponse(packet.getSocketAddress(), ret.toString());
					break;
				case UREAD:
					ret = locker.unlockRead(lMsg.getAccountId());
					Message.sendResponse(packet.getSocketAddress(), ret.toString());
					break;
				case UWRITE:
					ret = locker.unlockWrite(lMsg.getAccountId());
					Message.sendResponse(packet.getSocketAddress(), ret.toString());
					break;
				default:
					System.err.println("Unknown LockServer operation");
					break;
				
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
	
	/**
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		
		LockDeamon ld = new LockDeamon();
		try {
			ld.startDeamon();
			System.out.println("LockDeamon running... ");
		} catch (SocketException e) {
			e.printStackTrace();
		}
	}
	
	
}
