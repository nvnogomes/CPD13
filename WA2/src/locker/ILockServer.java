package locker;

import util.Status;

public interface ILockServer {

	/**
	 * 
	 * @param accountNum
	 * @return
	 */
	public Status lockWrite(int accountNum);

	/**
	 * 
	 * @param accountNum
	 * @return
	 */
	public Status unlockWrite(int accountNum);

	/**
	 * 
	 * @param accountNum
	 * @return
	 */
	public Status lockRead(int accountNum);

	/**
	 * 
	 * @param accountNum
	 * @return
	 */
	public Status unlockRead(int accountNum);

}
