package bank;

import util.Status;

public class AccountManagement implements IBankServer {

	/**
	 * operations offered by the bank
	 */
	public static enum BANKOP {
		WRITE, READ,
	}

	/**
	 * number of accounts
	 */
	public final static int NACCOUNTS = 512;

	/**
	 * array with the total of the accounts
	 */
	private int[] accounts;

	/**
	 * Singleton instance
	 */
	private static AccountManagement instance;

	/**
	 * Default Constructor
	 */
	private AccountManagement() {
		this.accounts = new int[NACCOUNTS];
	}

	/**
	 * Singleton Pattern
	 * 
	 * @return AccountManagement
	 */
	public static AccountManagement getInstance() {
		if (instance == null) {
			instance = new AccountManagement();
		}
		return instance;
	}

	/**
	 * Checks if the account is within bounds
	 * 
	 * @param id
	 * @return
	 */
	private boolean existsAccount(int id) {
		return id >= 0 && id < NACCOUNTS;
	}

	/**
	 * Updates the account value
	 * 
	 * @param account_id
	 * @param value
	 * @return Status
	 */
	public Status writeAccount(int account_id, int value) {

		if (existsAccount(account_id)) {
			this.accounts[account_id] = value;

			if (this.accounts[account_id] == value) {
				return new Status(0, value);
			}
		}

		return new Status(1);
	}

	/**
	 * Reads the value of the account
	 * 
	 * @param account_id
	 * @return status
	 */
	public Status readAccount(int account_id) {

		if (existsAccount(account_id)) {
			return new Status(0, this.accounts[account_id]);
		}

		return new Status(1);
	}

}
