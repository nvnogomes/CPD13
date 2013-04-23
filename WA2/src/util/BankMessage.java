package util;

import bank.AccountManagement.BANKOP;

public class BankMessage {
	
	
	private BANKOP op;
	
	private int accountId;
	
	private int value;

	
	public BankMessage() {
	
	}
	
	
	public BankMessage(BANKOP op, int accountId, int value) {
		super();
		this.op = op;
		this.accountId = accountId;
		this.value = value;
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
	
	
	public void build(String text) {
		String[] split = text.split(" ");
		
		this.op = BANKOP.valueOf(split[0]);
		this.accountId = Integer.parseInt(split[1]);
		this.value = Integer.parseInt(split[2]);
	}
	
	@Override
	public String toString() {
		return op +" "+ accountId +" "+ value;
	}
	
	

}
