package util;

import locker.LockServer.LOCKEROP;

public class LockerMessage {
	
	private LOCKEROP op;
	private int accountId;
	
	
	
	public LockerMessage() {}
	
	
	public LockerMessage(LOCKEROP op, int accountId) {
		super();
		this.op = op;
		this.accountId = accountId;
	}

	public LOCKEROP getOp() {
		return op;
	}

	public void setOp(LOCKEROP op) {
		this.op = op;
	}

	public int getAccountId() {
		return accountId;
	}

	public void setAccountId(int accountId) {
		this.accountId = accountId;
	}

	public void build(String text) {
		String[] split = text.split(" ");
		
		this.op = LOCKEROP.valueOf(split[0]);
		this.accountId = Integer.parseInt(split[1]);
	}
	
	@Override
	public String toString() {
		return op +" "+ accountId;
	}

}
