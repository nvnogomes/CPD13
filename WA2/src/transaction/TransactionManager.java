package transaction;

import java.util.HashMap;
import java.util.Map;

/**
 * Transaction Manager implementation
 * 
 * Decision to be made.
 * 1 - Strategy (locks, timestamp?)
 *
 */
public class TransactionManager implements ITransactionServer {

	/**
	 * 
	 *
	 */
	protected static enum TRANSOP {
		BEGIN,
		COMMIT,
		ABORT,
		READ,
		WRITE,
	}
	
	/**
	 * 
	 */
	protected Map<Integer,Transaction> log;
	

	/**
	 * 
	 */
	private int currentId;
	
	/**
	 * Singleton instance
	 */
	private static TransactionManager instance;
	
	
	/**
	 * 
	 */
	private TransactionManager() {
		log = new HashMap<Integer,Transaction>();
		currentId = 0;
	}
	
	/**
	 * 
	 * @return
	 */
	public static TransactionManager getInstance() {
		if( instance == null ) {
			instance = new TransactionManager();
		}
		
		return instance;
	}
	
	private int getNextId() {
		currentId++;
		return currentId;
	}
	
	
	@Override
	public int beginT() {
		int id = getNextId();
		log.put(id, new Transaction());
		
		return id;
	}

	@Override
	public int commitT(int transactionId) {
		// TODO
		
		// schedule events?
		return 0;
	}

	@Override
	public int abortT(int transactionId) {
		// TODO
		
		// restore old values
		return 0;
	}

	@Override
	public int readT(int accontNum) {
		// TODO
		
		// get Lock
		
		
		// log event
		log.get(currentId).addRead(accontNum);
		return 0;
	}

	@Override
	public int writeT(int accontNum, int balance) {
		// TODO
		// get old value
		int oldVal = 0;
		
		// get write lock
		
		// log event
		log.get(currentId).addWrite(accontNum, oldVal, balance);
		
		return 0;
	}

}
