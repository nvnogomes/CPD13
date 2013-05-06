package bank;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

import util.BankMessage;
import util.Converter;
import util.Message;
import util.Status;




public class BankDeamon extends Thread {
	
	/**
	 * Deamon port.
	 */
	public final static int PORT = 47777;

	
	/**
	 * Datagram socket.
	 */
	private DatagramSocket dSocket;
	
	
	/**
	 * Bank accounts
	 */
	private AccountManagement bank;
	
	
	/**
	 * Status of the deamon
	 */
	private boolean active;
	
	
	/**
	 * 
	 * @param am Dependency Injection
	 */
	public BankDeamon(AccountManagement am) {
		bank = am;
	}
	
	/**
	 * 
	 */
	public void startDeamon() throws SocketException {
		this.dSocket = new DatagramSocket(PORT);
		this.active = true;
		start();
	}
	
	public void stopDeamon() {
		this.active = false;
		this.dSocket.close();
	}
	
	/**
	 * Thread code
	 */
	public void run() {

		while ( active ) {
			byte[] buffer = new byte[2048];
			DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
			BankMessage bMsg;
			Status ret;
			try {
				dSocket.receive(packet);
				bMsg = BankMessage.build( Converter.byteArrayToString(buffer) );
				
				switch( bMsg.getOp() ) {
				case READ:
					ret = bank.readAccount(bMsg.getAccountId());
					System.out.println("READ OPERATION: "+ ret.getStat() +" "+ ret.getValue());
					Message.sendResponse(packet.getSocketAddress(), ret.toString());
					break;
				case WRITE:
					ret = bank.writeAccount(bMsg.getAccountId(), bMsg.getValue());
					System.out.println("WRITE OPERATION: "+ ret.getStat() +" "+ ret.getValue());
					Message.sendResponse(packet.getSocketAddress(), ret.toString());
					break;
				default:
					System.err.println("Unkonwn AccountManager operation");
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
		
		BankDeamon ld = new BankDeamon( AccountManagement.getInstance() );
		try {
			ld.startDeamon();
			System.out.println("BankDeamon running... ");
		} catch (SocketException e) {
			e.printStackTrace();
		}
	}
}
