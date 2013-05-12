package transaction;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import locker.LockServer;
import util.Status;
import util.Status.Result;
import bank.AccountManagement;
import bank.IBankServer;

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
    protected LockServer ls;
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
		//TODO

		Transaction t = log.get(transactionId);
		Iterator<TransactLog> it = t.getIterator();
		
		TransactLog tl = null;
		Result result = Status.Result.OK;
		
		while(it.hasNext()){
			tl = it.next();
			
			if(tl.isWriteOperation()){
				result = ls.unlockWrite(tl.accountId).getStat();
			}else{
				result = ls.unlockRead(tl.accountId).getStat();
			}
			
			if(result != Result.OK){
				//algum unlock falhado
				return -1;
			}
		}
		
		return 1;
	}

	@Override
	public int abortT(int transactionId) {
		// TODO
		
		Transaction t = log.get(transactionId);
		Iterator<TransactLog> it = t.getIterator();
		
		TransactLog tl = null;
		Result result = Status.Result.OK;
		
		while(it.hasNext()){
			tl = it.next();
			
			if(tl.isWriteOperation()){
				result = acm.writeAccount(tl.accountId, tl.oldValue).getStat();
			}
			
			if(result != Result.OK){
				//algum write falhado
				return -1;
			}
		}
		
		while(it.hasNext()){
			tl = it.next();
			
			if(tl.isWriteOperation()){
				result = ls.unlockWrite(tl.accountId).getStat();
			}
			
//			else{
//				result = ls.unlockRead(tl.accountId).getStat();
//			}
			
			if(result != Result.OK){
				//algum unlock falhado
				return -1;
			}
		}
		
		
		
		// restore old values
		return 1;
	}

	@Override
	public int readT(int accontNum) {
		// TODO
		
        if(ls.lockRead(accontNum).getStat() == Result.OK){
    		log.get(currentId).addRead(accontNum);
    		
    		return 1;
        }
		return 0;
	}

	@Override
	public int writeT(int accontNum, int balance) {
		// TODO
		
		int oldVal = 0;
		if(ls.lockWrite(accontNum).getStat() == Result.OK){
			oldVal = acm.readAccount(accontNum).getValue();
			
			log.get(currentId).addWrite(accontNum, oldVal, balance);
			
			return 1;
		}
		return 0;
	}

}
