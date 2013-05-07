package util;

public class Debug {

	/**
	 * State of debug messages
	 */
	private boolean isActive;

	/**
	 * Field Constructor
	 * 
	 * @param state
	 */
	public Debug(boolean state) {
		this.isActive = state;
	}

	/**
	 * Prints the messages if active
	 * 
	 * @param str
	 */
	public void show(String str) {
		if (isActive) {
			System.out.println(str);
		}
	}

}
