package transaction;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;


/**
 * This class represents a single transaction operation 
 */
class TransactLog {
	
	/**
	 *
	 */
	protected int accountId;
	
	/**
	 * 
	 */
	private char operation; 

	/**
	 * 
	 */
	protected int oldValue;
	
	/**
	 * 
	 */
	protected int newValue;

	
	private TransactLog(char op, int accountId, int oldValue, int newValue) {
		super();
		this.operation = op;
		this.accountId = accountId;
		this.oldValue = oldValue;
		this.newValue = newValue;
	}
	
	public static TransactLog LogWrite(int accId, int oldVal, int newVal) {
		return new TransactLog('w',accId,oldVal,newVal);
	}
	
	public static TransactLog LogRead(int accId) {
		return new TransactLog('r',accId,0,0);
	}
	
	public boolean isWriteOperation() {
		return this.operation == 'w';
	}
}


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
