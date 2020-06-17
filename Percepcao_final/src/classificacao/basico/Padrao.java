package classificacao.basico;

import java.util.Vector;

public class Padrao {

	public Vector<Double> atributos;
	public String classe;
	public double distancia;
	
	public Padrao(int nroCaracteristicas){
		this.atributos = new Vector<Double>(nroCaracteristicas);
		this.distancia = 0.0;
	}
}
