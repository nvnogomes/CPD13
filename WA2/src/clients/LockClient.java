package clients;

import locker.LockMessage;
import locker.LockServer.LOCKOP;
import util.Status;

public class LockClient {

	
	public Status lockWrite(int accountNum) throws Exception {
		LockMessage msg = new LockMessage(LOCKOP.LWRITE, accountNum);

		Status ret = msg.send();

		return ret;
	}

	public Status unlockWrite(int accountNum) throws Exception {

		LockMessage msg = new LockMessage(LOCKOP.UWRITE, accountNum);

		Status ret = msg.send();

		return ret;
	}

	public Status lockRead(int accountNum) throws Exception {
		LockMessage msg = new LockMessage(LOCKOP.LREAD, accountNum);

		Status ret = msg.send();

		return ret;
	}

	public Status unlockRead(int accountNum) throws Exception {
		LockMessage msg = new LockMessage(LOCKOP.UREAD, accountNum);

		Status ret = msg.send();

		return ret;
	}

}
