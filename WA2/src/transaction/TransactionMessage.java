package transaction;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;

import transaction.TransactionManager.TRANSOP;
import util.Converter;
import util.Message;
import util.Status;

public class TransactionMessage extends Message {

	/**
	 * 
	 */
	private TRANSOP op;

	/**
	 * 
	 */
	private int accountId;

	/**
	 * 
	 */
	private int value;

	/**
	 * 
	 */
	public TransactionMessage() {
	}

	/**
	 * 
	 * @param op
	 * @param account
	 * @param value
	 */
	public TransactionMessage(TRANSOP op, int account, int value) {
		this.op = op;
		this.accountId = account;
		this.value = value;
	}

	/**
	 * 
	 * @return
	 */
	public TRANSOP getOp() {
		return op;
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
	 * @return
	 */
	public int getValue() {
		return value;
	}

	/**
	 * 
	 * @param text
	 * @return
	 */
	public static TransactionMessage build(String text) {
		String[] split = text.split(" ");

		TRANSOP op = null;
		try {
			op = TRANSOP.valueOf(split[0].toUpperCase());
		} catch (Exception e) {
			return null;
		}

		int account = Integer.parseInt(split[1]);

		int value = 0;
		if (split.length == 3) {
			String valueStr = split[2].replaceAll("[^0-9]", ""); // bug fix
			value = Integer.parseInt(valueStr);
		}

		return new TransactionMessage(op, account, value);
	}
	
	@Override
	public String toString() {
		return op +" "+ accountId +" "+ value;
	}
	
	
	/**
	 * Sends message to the Transaction deamon and waits for response
	 * 
	 * @return
	 * @throws UnknownHostException
	 */
	public Status send() throws UnknownHostException {

		InetAddress ip = InetAddress.getByName("127.0.0.1");
		InetSocketAddress isa = new InetSocketAddress(ip, TransactionDeamon.PORT);

		byte[] ret = Message.sendOperation(isa, this.toString());
		return Status.build(Converter.byteArrayToString(ret));
	}
}
