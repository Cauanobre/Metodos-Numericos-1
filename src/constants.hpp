#pragma once
 
// Constantes compartilhadas pelos métodos
#define MAX_ITER   100      // limite de iterações
#define MAX_CM     0.7      // deslocamento crítico, acima disso o movimento amplifica
#define TOL_DERIV  1e-12    // |f'(x0)| abaixo disso é tratado como zero
#define PASSO_SECANTE 0.1   // segundo ponto inicial da secante: d1 = d0 + PASSO_SECANTE