package locker;

import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

import util.Status;
import bank.AccountManagement;

public class LockServer {

	private final int TIMEOUT = 10;
	private final TimeUnit UNIT = TimeUnit.SECONDS;
	
	public enum LOCKEROP {
		LWRITE,
		UWRITE,
		LREAD,
		UREAD,
	}
	
	
	private ReentrantLock mutex = new ReentrantLock();
	
	
	/**
	 * Lock status of each account
	 */
	private ReentrantReadWriteLock[] accounts;
	
	
	public LockServer() {
		accounts = new ReentrantReadWriteLock[AccountManagement.NACCOUNTS];
	}
	
	
	public Status lockWrite(int lockAcc) throws InterruptedException {
		int stat = 0;
		
		mutex.lock();
		boolean acquired = accounts[lockAcc].writeLock().tryLock();
		if( acquired == false ) {
			acquired = accounts[lockAcc].writeLock().tryLock(TIMEOUT, UNIT);
			if( acquired == false ){
				stat = 1;
			}
		}
		mutex.unlock();
		
		return new Status( stat );
	}
	
	public Status unlockWrite(int lockAcc) {
		int stat;
		
		mutex.lock();
		if( accounts[lockAcc].writeLock().getHoldCount() == 0 ){
			stat = 1;
		}
		else {
			accounts[lockAcc].writeLock().unlock();
			stat = 0;
		}
		mutex.unlock();
			
		return new Status( stat );
	}
	
	
	public Status lockRead(int lockAcc) throws InterruptedException {
		int stat = 0;
		
		mutex.lock();
		boolean acquired = accounts[lockAcc].readLock().tryLock();
		if( acquired == false ) {
			acquired = accounts[lockAcc].readLock().tryLock(TIMEOUT, UNIT);
			if( acquired == false ){
				stat = 1;
			}
		}
		mutex.unlock();
		
		return new Status( stat );
	}
	
	public Status unlockRead(int lockAcc) {
		
		mutex.lock();
		accounts[lockAcc].readLock().unlock();
		mutex.unlock();
		
		return new Status(0);
	}
	
}
