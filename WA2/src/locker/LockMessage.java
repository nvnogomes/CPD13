package locker;

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
	public Status send() throws UnknownHostException {
		InetAddress ip = InetAddress.getByName("127.0.0.1");
		InetSocketAddress isa = new InetSocketAddress(ip, LockDeamon.PORT);

		byte[] ret = Message.sendOperation(isa, this.toString());
		return Status.build(Converter.byteArrayToString(ret));
	}

	@Override
	public String toString() {
		return op + " " + accountId;
	}

}
