package bank;

import util.Status;

public interface IBankServer {

	public Status readAccount(int accountNum);
	
	public Status writeAccount(int accountNum, int value);
	
}
