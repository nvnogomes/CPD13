package locker;

import util.Status;
import bank.AccountManagement;

public class LockServer {

	public enum LOCKEROP {
		LWRITE,
		UWRITE,
		LREAD,
		UREAD,
	}
	
	
	/**
	 * Lock status enum
	 */
	private enum Lock {
		NONE,
		READ,
		WRITE,
	}
	
	/**
	 * Lock status of each account
	 */
	private Lock[] accounts;
	
	
	public LockServer() {
		accounts = new Lock[AccountManagement.NACCOUNTS];
	}
	
	
	public Status lockWrite(int lockAcc) {
		
		return new Status(0);
	}
	
	public Status unlockWrite(int lockAcc) {
		
		return new Status(0);
	}
	
	public Status lockRead(int lockAcc) {
		
		return new Status(0);
	}
	
	public Status unlockRead(int lockAcc) {
		
		return new Status(0);
	}
	
}
