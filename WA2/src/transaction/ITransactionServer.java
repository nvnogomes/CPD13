package transaction;

public interface ITransactionServer {

	/**
	 * inicia uma transacção. 
	 * Devolve o identificador da transacção em caso de êxito 
	 * e -1 em caso de erro.
	 * 
	 * @return
	 */
	public int beginT();
	
	
	/**
	 * Termina a transacção identificado por trid fazendo commit. 
	 * Devolve 0 se a operação foi realizada com êxito ou -1 em caso de erro.
	 * 
	 * @param transactionId
	 * @return
	 */
	public int commitT(int transactionId);
	
	
	/**
	 * Termina a transacção identificado por trid fazendo abort. 
	 * Devolve 0 se a operação foi realizada com êxito ou -1 em caso de	erro.
	 * 
	 * @param transactionId
	 * @return
	 */
	public int abortT (int transactionId);
	
	/**
	 * Realiza uma operação de leitura de um saldo da conta nConta. 
	 * Contém a realização de um lock de leitura sobre a conta nConta. 
	 * Se o lock não for conseguido por timeout a operação retorna -1.
	 * 
	 * @param accontNum
	 * @return
	 */
	public int readT(int accontNum);
	
	/**
	 * Realiza uma operação de escrita de um saldo da conta nConta. 
	 * Contém a realização de um lock de escrita sobre a conta nConta. 
	 * Se o lock não for conseguido por timeout a operação retorna -1.
	 * 
	 * @param accontNum
	 * @param balance
	 * @return
	 */
	public int writeT (int accontNum, int balance);
	
}
