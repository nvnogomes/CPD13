package transaction;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import locker.ILockServer;
import locker.LockServer;
import util.Status;
import bank.AccountManagement;
import bank.IBankServer;

/**
 * Transaction Manager implementation
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
    protected ILockServer ls;
    
    /**
     * 
     */
    protected IBankServer acm;
	

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

        //lockServer
        ls = LockServer.getInstance();
        
        //AccountManager
        acm = AccountManagement.getInstance();

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
	
	
	public void reset() {
		instance = new TransactionManager();
	}
	
	
	/**
	 * 
	 * @return
	 */
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

		Transaction t = log.get(transactionId);
		Iterator<TransactLog> it = t.getIterator();
		
		TransactLog tl = null;
		
		while(it.hasNext()){
			tl = it.next();
			
			Status opResult;
			if(tl.isWriteOperation()){
				opResult = ls.unlockWrite(tl.accountId);
			}
			else{
				opResult = ls.unlockRead(tl.accountId);
			}
			
			// something wrong
			if(opResult.isSuccessful() == false ){
				return -1;
			}
		}
		
		return 0;
	}

	@Override
	public int abortT(int transactionId) {
		
		Transaction t = log.get(transactionId);
		Iterator<TransactLog> it = t.getIterator();
		
		TransactLog tl = null;
		
		while(it.hasNext()){
			tl = it.next();
			
			if(tl.isWriteOperation()){
				int writeStat = writeT(tl.accountId, tl.oldValue);
				
				// retry
				if( writeStat == -1 ) {
					try {
						Thread.sleep(2000);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					abortT(transactionId);
				}
			}
		}
		
		// restore old values
		return 0;
	}

	@Override
	public int readT(int accountNum) {

		// register the operation
		if( log.containsKey(currentId) ) {
			Transaction t = log.get(currentId);
			t.addRead(accountNum);
		}
		else {
			System.err.println("NOP");
			System.out.println( log.size() );
			System.exit(1);
		}
		
        if(ls.lockRead(accountNum).isSuccessful() ){
        	return 0;
        }
		return -1;
	}

	
	@Override
	public int writeT(int accountNum, int balance) {
		
		// get old value through a readT
		int oldVal = readT(accountNum);
		if( oldVal == -1 ) {
			return -1;
		}
		
		// register operation
		log.get(currentId).addWrite(accountNum, oldVal, balance);
		
		// acquire write lock and update account. write unlock account
		if( ls.lockWrite(accountNum).isSuccessful() ){
			return 0;
		}
		return -1;
	}

}
