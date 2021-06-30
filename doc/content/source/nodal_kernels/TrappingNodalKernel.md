# TrappingNodalKernel

!syntax description /NodalKernels/TrappingNodalKernel

## Overview

This object implements the residual

\begin{equation}
\frac{\alpha_t}{N} C_t^e C_s
\end{equation}

where $\alpha_t$ is the trapping rate coefficient, which has dimensions of
1/time, $N$ is the atomic number density of the host material, $C_t^e$ is the
concentration of empty trapping sites, and $C_s$ is the concentration of the
mobile species.

As outlined in [content/index.md#scaling] it is important to scale different
specie numerical concentrations to the same order of magnitude in order to have
robust (non)linear solves. Unfortunately, incorporation of scaling tends to
obfuscate residual-computing objects a bit, including
`TrappingNodalKernel`. Hopefully we can make things clear here, however. Let's
imagine that the trapped concentration is 1000 times greater than the mobile
concentration. We can bring the trapped concentration to the same numerical
level of the mobile concentration by changing the trapped concentration units
from #/volume to k#/volume where 'k' denotes kilo. Now let's figure out how this
transformation fits into `TrappingNodalKernel`. `TrappingNodalKernel` is
producing trapped species, so consequently its residual must have units (in our
example) of k#/(volume*time). Trapped concentrations have units of k#/volume
and the mobile concentration and host density ($N$) have units of
#/volume. `TrappingNodalKernel` computes the empty trapping sites concentration,
$C_t^e$ in the following way:

```language=c++
  auto empty_trapping_sites = _Ct0 * _N;
  for (const auto & trap_conc : _trapped_concentrations)
    empty_trapping_sites -= (*trap_conc)[_qp] * _trap_per_free;
```

The trapping concentration, in units of k#/volume, is converted to units of
#/volume by multiplying by `trap_per_free` which, in this example of k#/volume
trapping concetration and #/volume mobile concentration, has a value of
1000 #/(k#). We then compute the residual with the code

```language=c++
  return -_alpha_t * empty_trapping_sites * _mobile_conc[_qp] / (_N * _trap_per_free);
```

Let's carry through the units: 1/time * #/volume * #/volume / (#/volume * 1000 #/(k#)) ->
k#/(time*volume) which is exactly the units that we needed.

!syntax parameters /NodalKernels/TrappingNodalKernel

!syntax inputs /NodalKernels/TrappingNodalKernel

!syntax children /NodalKernels/TrappingNodalKernel
