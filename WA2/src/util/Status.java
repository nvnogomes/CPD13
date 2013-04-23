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


	public Result getStat() {
		return stat;
	}


	public int getValue() {
		return value;
	}


	public void build(String text) {
		String[] split = text.split(" ");
		
		this.stat = Result.valueOf(split[0]);
		this.value = Integer.parseInt(split[1]);
	}
	
	@Override
	public String toString() {
		return stat +" "+ value;
	}
	
	
}
