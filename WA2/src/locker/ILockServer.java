package locker;

import util.Status;

public interface ILockServer {

	
	public void reset();
	
	public Status lockWrite(int accountNum);
	
	public Status unlockWrite(int accountNum);
	
	public Status lockRead(int accountNum) throws InterruptedException;
	
	public Status unlockRead(int accountNum);
	
	
}
