package util;

public class Status {
	
	public enum Result {
		OK,
		NOT_OK,
	}	
	
	public Result stat; 
	public int value;

	
	public Status(int stat) {
		this(stat,0);
	}
	
	
	public Status(int stat, int value) {
		super();
		this.stat = Result.values()[stat];
		this.value = value;
	}
	
	public Status(Result stat, int value) {
		super();
		this.stat = stat;
		this.value = value;
	}


	public Result getStat() {
		return stat;
	}


	public int getValue() {
		return value;
	}


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
