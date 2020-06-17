package classificacao.knn;

import java.util.ArrayList;
import java.util.Collections;

import util.ComparadorExemplo;
import util.Matematica;
import classificacao.basico.Base;
import classificacao.basico.Padrao;

public class KNN {

	public static String k_nn(int k, Padrao novo, Base base, boolean peso){
		ArrayList<Padrao> vizinhos = new ArrayList<Padrao>(base.padroes.size());
		for (int i = 0; i < base.padroes.size(); i++) {

			Padrao temp = base.padroes.elementAt(i);
			//temp.distancia = Matematica.euclidiana(novo, base.padroes.elementAt(i));
			temp.distancia = Matematica.euclidianaNormalizada(novo, base.padroes.elementAt(i), base.maxValues, base.minValues);
			//temp.distancia = Matematica.manhattan(novo, base.padroes.elementAt(i));
			vizinhos.add(temp);

		}
		Collections.sort(vizinhos, new ComparadorExemplo());
		
		ArrayList<ArrayList<Padrao>> vizinhosAgrupados = new ArrayList<ArrayList<Padrao>>();
		for (int i = 0; i < k; i++) {
			boolean achou = false;
			for (int j = 0; j < vizinhosAgrupados.size(); j++) {
				ArrayList<Padrao> grupoTemp = vizinhosAgrupados.get(j);
				if(grupoTemp.get(0).classe.equals(vizinhos.get(i).classe)){
					grupoTemp.add(vizinhos.get(i));
					achou = true;
				}
			}
			if(!achou){
				ArrayList<Padrao> novoGrupo = new ArrayList<Padrao>();
				novoGrupo.add(vizinhos.get(i));
				vizinhosAgrupados.add(novoGrupo);
			}
		}
		
		ArrayList<Padrao> maior = new ArrayList<Padrao>(0);
		if(peso){
			double menorDist = Double.MIN_VALUE;
			for (int i = 0; i < vizinhosAgrupados.size(); i++) {
				double distGroup = 0;
				for(int j = 0; j < vizinhosAgrupados.get(i).size(); j++ ){
					distGroup += 1/vizinhosAgrupados.get(i).get(j).distancia;
				}
				if(menorDist < distGroup){
					menorDist = distGroup;
					maior = vizinhosAgrupados.get(i);
				}
			}
		}else{
			for (int i = 0; i < vizinhosAgrupados.size(); i++) {
				if(vizinhosAgrupados.get(i).size() > maior.size()){
					maior = vizinhosAgrupados.get(i);
				}
			}
		}
		return maior.get(0).classe;
	}
	
}
