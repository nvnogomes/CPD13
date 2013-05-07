package bank;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;

import util.Converter;
import util.Message;
import util.Status;
import bank.AccountManagement.BANKOP;

public class BankMessage extends Message {

	/**
	 * 
	 */
	private BANKOP op;

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
	 * @param op
	 * @param account
	 * @param value
	 */
	public BankMessage(BANKOP op, int account, int value) {
		super();
		this.op = op;
		this.accountId = account;
		this.value = value;
	}

	/**
	 * 
	 * @param accountId
	 * @param value
	 * @return
	 */
	public static BankMessage writeMessage(int accountId, int value) {
		return new BankMessage(BANKOP.WRITE, accountId, value);
	}

	/**
	 * 
	 * @param accountId
	 * @return
	 */
	public static BankMessage readMessage(int accountId) {
		return new BankMessage(BANKOP.READ, accountId, 0);
	}

	/**
	 * 
	 * @return
	 */
	public BANKOP getOp() {
		return op;
	}

	/**
	 * 
	 * @param op
	 */
	public void setOp(BANKOP op) {
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
	 * @return
	 */
	public int getValue() {
		return value;
	}

	/**
	 * 
	 * @param value
	 */
	public void setValue(int value) {
		this.value = value;
	}

	/**
	 * 
	 * @param text
	 * @return
	 */
	public static BankMessage build(String text) {
		String[] split = text.split(" ");

		BANKOP op = BANKOP.valueOf(split[0]);
		int accountId = Integer.parseInt(split[1]);
		String valueStr = split[2].replaceAll("[^0-9]", ""); // bug fix
		int value = Integer.parseInt(valueStr);

		return new BankMessage(op, accountId, value);
	}

	@Override
	public String toString() {
		return op + " " + accountId + " " + value;
	}

	/**
	 * Sends message to the bank deamon and waits for response
	 * 
	 * @return
	 * @throws UnknownHostException
	 */
	public Status send() throws UnknownHostException {

		InetAddress ip = InetAddress.getByName("127.0.0.1");
		InetSocketAddress isa = new InetSocketAddress(ip, BankDeamon.PORT);

		byte[] ret = Message.sendOperation(isa, this.toString());
		return Status.build(Converter.byteArrayToString(ret));
	}

}
