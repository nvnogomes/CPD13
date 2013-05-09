package transaction;

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
	 * Singleton instance
	 */
	private static TransactionManager instance;
	
	
	/**
	 * 
	 */
	private TransactionManager() {
		
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
	
	
	@Override
	public int beginT() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public int commitT(int transactionId) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public int abortT(int transactionId) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public int readT(int accontNum) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public int writeT(int accontNum, int balance) {
		// TODO Auto-generated method stub
		return 0;
	}

}
