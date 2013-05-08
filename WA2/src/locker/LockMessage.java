package locker;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;

import locker.LockServer.LOCKOP;
import util.Converter;
import util.Message;
import util.Status;

public class LockMessage extends Message {

	/**
	 * 
	 */
	private LOCKOP op;

	/**
	 * 
	 */
	private int accountId;

	/**
	 * 
	 */
	public LockMessage() {
	}

	/**
	 * 
	 * @param op
	 * @param accountId
	 */
	public LockMessage(LOCKOP op, int accountId) {
		super();
		this.op = op;
		this.accountId = accountId;
	}

	/**
	 * 
	 * @return
	 */
	public LOCKOP getOp() {
		return op;
	}

	/**
	 * 
	 * @param op
	 */
	public void setOp(LOCKOP op) {
		this.op = op;
	}

	/**
	 * 
	 * @return
	 */
	public int getAccountId() {
		return accountId;
	}

	/**
	 * 
	 * @param accountId
	 */
	public void setAccountId(int accountId) {
		this.accountId = accountId;
	}

	/**
	 * 
	 * @param text
	 * @return
	 */
	public static LockMessage build(String text) {
		String[] split = text.split(" ");

		LOCKOP op = LOCKOP.valueOf(split[0]);
		String valueStr = split[1].replaceAll("[^0-9]", ""); // bug fix
		int account = Integer.parseInt(valueStr);

		return new LockMessage(op, account);
	}

	/**
	 * 
	 * @return
	 * @throws UnknownHostException
	 */
	public Status send() throws Exception {
		InetAddress ip = InetAddress.getByName("127.0.0.1");
		InetSocketAddress isa = new InetSocketAddress(ip, LockDeamon.PORT);

		DatagramSocket dSocket = new DatagramSocket();

		byte[] buf = toString().getBytes();
		DatagramPacket resp = new DatagramPacket(buf, buf.length, isa);
		dSocket.send(resp);

		byte[] buffer = new byte[2048];
		DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
		
		try {
			dSocket.setSoTimeout(Message.TIMEOUT);
			dSocket.receive(packet);
			dSocket.close();
		} catch (Exception e) {
			return new Status(1);
		}

		return Status.build(Converter.byteArrayToString(buffer));
	}

	@Override
	public String toString() {
		return op + " " + accountId;
	}

}
