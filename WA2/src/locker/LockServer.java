package locker;

import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

import util.Status;
import bank.AccountManagement;

public class LockServer implements ILockServer {

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
	 * Start a new object
	 */
	public void reset() {
		instance = new LockServer();
	}

	/**
	 * Checks if the given account number is within limits
	 * 
	 * @param account
	 *            number
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
	 * Lock Write in the given account number if the thread that holds the lock
	 * requires it again it will return a success operation
	 * 
	 * @param lockAcc
	 *            account number
	 * @return Status object
	 * @throws InterruptedException
	 */
	public Status lockWrite(int lockAcc) {
		int stat = 1;

		if (isValidAccount(lockAcc)) {
			mutex.lock();
			if (accounts[lockAcc].isWriteLockedByCurrentThread()) {
				stat = 0; // thread already holds the lock
			} else {
				boolean acquired = false;
				try {
					acquired = accounts[lockAcc].writeLock().tryLock(TIMEOUT,
							UNIT);
				} catch (InterruptedException e) {
					stat = 1;
				}
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
	 * @param lockAcc
	 *            account number
	 * @return Status
	 */
	public Status unlockWrite(int lockAcc) {
		int stat = 1;

		if (isValidAccount(lockAcc)) {
			mutex.lock();
			if (accounts[lockAcc].isWriteLockedByCurrentThread()) {
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
	 * Locks the account for reading NOTE: By the ReentrantReadWriteLock
	 * implementation, the thread that holds the write lock can also acquire the
	 * read lock. So in our program if the thread holds the write lock, it
	 * CANNOT have acquire the read lock.
	 * 
	 * @param lockAcc
	 *            account number
	 * @return Status
	 * @throws InterruptedException
	 */
	public Status lockRead(int lockAcc) {
		int stat = 1;

		if (isValidAccount(lockAcc)) {
			mutex.lock();
			if (accounts[lockAcc].isWriteLockedByCurrentThread()) {
				stat = 1;
			} else if (accounts[lockAcc].getReadHoldCount() > 0) {
				stat = 0;
			} else {
				boolean acquired = false;

				try {
					acquired = accounts[lockAcc].readLock().tryLock(TIMEOUT,
							UNIT);
				} catch (InterruptedException e) {
					stat = 1;
				}

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
	 * @param lockAcc
	 *            account number
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
