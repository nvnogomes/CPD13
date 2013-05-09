package clients;

import java.net.UnknownHostException;

import bank.BankMessage;
import util.Status;

public class BankClient {

	
	public BankClient() {
	
	}
	
	public Status writeOperation(int account, int value)
			throws UnknownHostException {

		BankMessage msg = BankMessage.writeMessage(account, value);

		Status stat = msg.send();

		return stat;
	}

	public Status readOperation(int account) throws UnknownHostException {

		BankMessage msg = BankMessage.readMessage(account);

		Status stat = msg.send();

		return stat;
	}

}
