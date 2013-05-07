package util;

public class Converter {

	/**
	 * Returns a string from a given length byte array.
	 * 
	 * @param data
	 *            : byte array to convert
	 * @param length
	 *            : number of bytes present in the data
	 * @return returns a string representation of the data
	 */
	public static String byteArrayToString(byte[] bytes, int length) {
		char[] chars = new char[length];
		for (int i = 0; i != chars.length; i++)
			chars[i] = (char) (bytes[i] & 0xff);

		return new String(chars);
	}

	/**
	 * Returns a string from a given byte array.
	 * 
	 * @param data
	 *            : byte array to convert
	 * @return returns a string representation of the data
	 */
	public static String byteArrayToString(byte[] bytes) {
		return byteArrayToString(bytes, bytes.length);
	}

}
