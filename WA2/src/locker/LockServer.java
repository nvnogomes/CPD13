package locker;

import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

import util.Status;
import bank.AccountManagement;

public class LockServer {

	/**
	 * Setting timeout for the lock attempt
	 */
	private final int TIMEOUT = 2;
	private final TimeUnit UNIT = TimeUnit.SECONDS;

	/**
	 * Operations recognized by the server
	 */
	public enum LOCKOP {
		LWRITE, UWRITE, LREAD, UREAD,
	}

	/**
	 * Singleton instance
	 */
	private static LockServer instance;

	/**
	 * Mutex crucial zones
	 */
	private ReentrantLock mutex = new ReentrantLock(true);

	/**
	 * Lock status of each account
	 */
	private ReentrantReadWriteLock[] accounts;

	
	/**
	 * Default Constructor
	 */
	private LockServer() {
		accounts = new ReentrantReadWriteLock[AccountManagement.NACCOUNTS];
		for (int i = 0; i < accounts.length; i++) {
			accounts[i] = new ReentrantReadWriteLock(true);
		}
	}

	/**
	 * Checks if the given account number is within limits
	 * 
	 * @param account number
	 * @return true if the number if valid, false otherwise
	 */
	private boolean isValidAccount(int account) {
		return account >= 0 && account < AccountManagement.NACCOUNTS;
	}

	/**
	 * Singleton Pattern
	 * 
	 * @return instance of the lock server
	 */
	public static LockServer getInstance() {
		if (instance == null) {
			instance = new LockServer();
		}
		return instance;
	}

	/**
	 * Lock Write in the given account number
	 * 
	 * @param lockAcc account number
	 * @return Status object
	 * @throws InterruptedException
	 */
	public Status lockWrite(int lockAcc) throws InterruptedException {
		int stat = 1;

		if (isValidAccount(lockAcc)) {
			mutex.lock();
			if (accounts[lockAcc].isWriteLockedByCurrentThread() == false) {
				boolean acquired;
				acquired = accounts[lockAcc].writeLock().tryLock(TIMEOUT, UNIT);
				if (acquired) {
					stat = 0;
				}
			}
			mutex.unlock();
		}

		return new Status(stat);
	}

	/**
	 * Unlocks the write operation of the account
	 * 
	 * @param lockAcc account number
	 * @return Status
	 */
	public Status unlockWrite(int lockAcc) {
		int stat = 1;

		if (isValidAccount(lockAcc)) {
			mutex.lock();
			if(accounts[lockAcc].isWriteLockedByCurrentThread() ) {
				if (accounts[lockAcc].isWriteLocked()) {
					accounts[lockAcc].writeLock().unlock();
					stat = 0;
				}
			}
			mutex.unlock();
		}

		return new Status(stat);
	}

	/**
	 * Locks the account for reading
	 * 
	 * @param lockAcc account number
	 * @return Status
	 * @throws InterruptedException
	 */
	public Status lockRead(int lockAcc) throws InterruptedException {
		int stat = 1;

		if (isValidAccount(lockAcc)) {
			mutex.lock();
			if (accounts[lockAcc].isWriteLockedByCurrentThread() == false) {
				boolean acquired;
				acquired = accounts[lockAcc].readLock().tryLock(TIMEOUT, UNIT);

				if (acquired) {
					stat = 0;
				}
			}
			mutex.unlock();
		}

		return new Status(stat);
	}

	/**
	 * Unlocks the read lock for the current thread, this if the lockAcc is
	 * valid and the current thread has a lock on it.
	 * 
	 * @param lockAcc account number
	 * @return
	 */
	public Status unlockRead(int lockAcc) {
		int stat = 1;

		if (isValidAccount(lockAcc)) {
			mutex.lock();
			if (accounts[lockAcc].getReadHoldCount() > 0) {
				accounts[lockAcc].readLock().unlock();
				stat = 0;
			}
			mutex.unlock();
		}

		return new Status(stat);
	}

}
