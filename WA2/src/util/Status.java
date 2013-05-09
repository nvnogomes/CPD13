package util;


public class Status {
	
	/**
	 * 
	 */
	public enum Result {
		OK,
		NOT_OK,
	}	
	
	/**
	 * 
	 */
	private Result stat;
	
	/**
	 * 
	 */
	private int value;

	/**
	 * 
	 * @param stat
	 */
	public Status(int stat) {
		this(stat,0);
	}
	
	
	/**
	 * 
	 * @param stat
	 * @param value
	 */
	public Status(int stat, int value) {
		super();
		this.stat = Result.values()[stat];
		this.value = value;
	}
	
	/**
	 * 
	 * @param stat
	 * @param value
	 */
	public Status(Result stat, int value) {
		super();
		this.stat = stat;
		this.value = value;
	}

	/**
	 * 
	 * @return
	 */
	public Result getStat() {
		return stat;
	}

	/**
	 * 
	 * @return
	 */
	public boolean isSuccess() {
		return stat == Result.OK; 
	}
	
	/**
	 * 
	 * @return
	 */
	public int getValue() {
		return value;
	}

	/**
	 * 
	 * @param text
	 * @return
	 */
	public static Status build(String text) {
		String[] split = text.split(" ");
		
		Result stat = Result.valueOf(split[0]);
		String valueStr = split[1].replaceAll("[^0-9]",""); // bug fix
		int value = Integer.parseInt(valueStr);
		
		return new Status(stat,value);
	}
	
	@Override
	public String toString() {
		return stat +" "+ value;
	}
	
	
}
