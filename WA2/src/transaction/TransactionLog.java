package transaction;

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

