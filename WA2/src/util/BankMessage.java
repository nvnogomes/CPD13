package util;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;

import bank.AccountManagement.BANKOP;
import bank.BankDeamon;

public class BankMessage {
	
	
	private BANKOP op;
	
	private int accountId;
	
	private int value;

	
	
	
	public BankMessage(BANKOP op, int account, int value) {
		super();
		this.op = op;
		this.accountId = account;
		this.value = value;
	}
	
	
	public static BankMessage writeMessage(int accountId, int value) {
		return new BankMessage(BANKOP.WRITE, accountId, value);
	}
	
	public static BankMessage readMessage(int accountId) {
		return new BankMessage(BANKOP.READ, accountId,0);
	}

	
	public BANKOP getOp() {
		return op;
	}

	public void setOp(BANKOP op) {
		this.op = op;
	}

	public int getAccountId() {
		return accountId;
	}

	public void setAccountId(int accountId) {
		this.accountId = accountId;
	}

	public int getValue() {
		return value;
	}

	public void setValue(int value) {
		this.value = value;
	}
	
	
	public static BankMessage build(String text) {
		String[] split = text.split(" ");
		
		BANKOP op = BANKOP.valueOf(split[0]);
		int accountId = Integer.parseInt(split[1]);
		String valueStr = split[2].replaceAll("[^0-9]",""); // bug fix
		int value = Integer.parseInt(valueStr);
		
		return new BankMessage(op, accountId, value);
	}
	
	@Override
	public String toString() {
		return op +" "+ accountId +" "+ value;
	}
	
	
	public Status send() throws UnknownHostException {
		
		InetAddress ip = InetAddress.getByName("127.0.0.1");
		InetSocketAddress isa = new InetSocketAddress(ip, BankDeamon.PORT);
		
		byte[] ret = Message.sendOperation(isa, this.toString());
		return Status.build( Converter.byteArrayToString(ret) );
	}

}
