package util;

import java.util.Comparator;

import classificacao.basico.Padrao;

public class ComparadorExemplo implements Comparator<Padrao>{

	public int compare(Padrao arg0, Padrao arg1) {
		double dif = arg0.distancia - arg1.distancia;
		if(dif > 0){
			return 1;
		}
		if(dif < 0){
			return -1;
		}
		return 0;
	}
}
