package transaction;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;


/**
 * This class represents the whole transaction, with all its operations
 *
 */
public class Transaction {
	
	/**
	 * 
	 */
	private List<TransactLog> transactOps;
	
	
	/**
	 * 
	 */
	public Transaction() {
		transactOps = new LinkedList<TransactLog>();
	}
	
	
	/**
	 * 
	 * @param accId
	 * @param oldVal
	 * @param newVal
	 */
	public void addWrite(int accId, int oldVal, int newVal) {
		transactOps.add( TransactLog.LogWrite(accId, oldVal, newVal));
	}
	
	/**
	 * 
	 * @param accId
	 */
	public void addRead(int accId) {
		transactOps.add( TransactLog.LogRead(accId));
	}
	
	/**
	 * 
	 * @return
	 */
	public Iterator<TransactLog> getIterator() {
		return transactOps.iterator();
	}
	
	
}
