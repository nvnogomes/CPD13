package util;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;

import locker.LockDeamon;
import locker.LockServer.LOCKEROP;

public class LockMessage {
	
	private LOCKEROP op;
	private int accountId;
	
	
	
	public LockMessage() {}
	
	
	public LockMessage(LOCKEROP op, int accountId) {
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

	
	public Status send() throws UnknownHostException {
		InetAddress ip = InetAddress.getByName("127.0.0.1");
		InetSocketAddress isa = new InetSocketAddress(ip, LockDeamon.PORT);
		
		byte[] ret = Message.sendOperation(isa, this.toString());
		return Status.build( Converter.byteArrayToString(ret) );
	}
	
}
