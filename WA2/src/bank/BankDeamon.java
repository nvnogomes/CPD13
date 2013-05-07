package bank;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

import util.Converter;
import util.Deamon;
import util.Debug;
import util.Message;
import util.Status;

public class BankDeamon extends Deamon {

	/**
	 * Deamon port.
	 */
	public static int PORT = 47777;

	/**
	 * Datagram socket.
	 */
	private DatagramSocket dSocket;

	/**
	 * Bank accounts
	 */
	private AccountManagement bank;

	/**
	 * Debug object
	 */
	private Debug debug;

	/**
	 * 
	 * Dependency Injection
	 * 
	 * @param am
	 *            AccountManagement object
	 */
	public BankDeamon(AccountManagement am) {
		this(am, new Debug(true));
	}

	/**
	 * Field constructor
	 * 
	 * Dependency Injection
	 * 
	 * @param am
	 *            AccountManagement object
	 * @param debugObj
	 *            Debug object
	 */
	public BankDeamon(AccountManagement am, Debug debugObj) {
		bank = am;
		debug = debugObj;
	}

	@Override
	public void startDeamon() throws SocketException {
		super.startDeamon();
		dSocket = new DatagramSocket(PORT);
	}

	@Override
	public void stopDeamon() {
		super.stopDeamon();
		dSocket.close();
	}

	/**
	 * Thread code
	 */
	public void routineCycle() {

		byte[] buffer = new byte[2048];
		DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
		BankMessage bMsg = null;
		Status ret = null;
		try {
			dSocket.receive(packet);
			bMsg = BankMessage.build(Converter.byteArrayToString(buffer));
			switch (bMsg.getOp()) {
			case READ:
				ret = bank.readAccount(bMsg.getAccountId());
				System.out.println("READ " + bMsg.getAccountId() + ": "
						+ ret.getStat() + " " + ret.getValue());
				Message.sendResponse(packet.getSocketAddress(), ret.toString());
				break;
			case WRITE:
				ret = bank.writeAccount(bMsg.getAccountId(), bMsg.getValue());
				System.out.println("WRITE " + bMsg.getAccountId() + ": "
						+ ret.getStat() + " " + ret.getValue());
				Message.sendResponse(packet.getSocketAddress(), ret.toString());
				break;
			default:
				ret = new Status(1);
				break;
			}

		} catch (Exception e) {
			e.printStackTrace();
		}
		debug.show(bMsg.getOp() + " " + bMsg.getAccountId() + " "
				+ bMsg.getValue() + " : " + ret.getStat() + " "
				+ ret.getValue());
	}

	/**
	 * Main
	 * 
	 * @param args
	 */
	public static void main(String[] args) {

		BankDeamon ld = new BankDeamon(AccountManagement.getInstance());
		try {
			ld.startDeamon();
			System.out.println("BankDeamon running... ");
		} catch (SocketException e) {
			e.printStackTrace();
		}
	}
}
