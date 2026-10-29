#pragma once
 
// Constantes compartilhadas pelos métodos
#define MAX_ITER   100      // limite de iterações
#define MAX_CM     0.7      // deslocamento crítico (cm): acima disso o movimento amplifica
#define TOL_DERIV  1e-12    // |f'(x0)| abaixo disso é tratado como zero
 