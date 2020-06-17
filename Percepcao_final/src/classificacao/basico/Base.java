package classificacao.basico;

import java.util.Vector;

public class Base {

	public Vector<Padrao> padroes;
	public Vector<Double> maxValues;
	public Vector<Double> minValues;
	//public Vector<Double> meanValues;
	
	public Base(int nroPadroes, int nroCaracteristicas){
		padroes = new Vector<Padrao>(nroPadroes);
		maxValues = new Vector<Double>(nroCaracteristicas);
		minValues = new Vector<Double>(nroCaracteristicas);
		//meanValues = new Vector<Double>(nroCaracteristicas);
		for (int i = 0; i < nroCaracteristicas; i++) {
			maxValues.add(i, Double.MIN_VALUE);
			minValues.add(i, Double.MAX_VALUE);
			//meanValues.add(i, 0.0);
		}
	}
	
}
