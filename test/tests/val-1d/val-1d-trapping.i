cl=3.1622e18
trap_per_free=1e3
N=3.1622e22
time_scaling=1
epsilon=10000

[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 20
  xmax = 1
[]

[Problem]
  type = ReferenceResidualProblem
  extra_tag_vectors = 'ref'
  reference_vector = 'ref'
[]

[Variables]
  [mobile][]
  [trapped][]
[]

[AuxVariables]
  [empty_sites][]
  [scaled_empty_sites][]
  [trapped_sites][]
  [total_sites][]
[]

[AuxKernels]
  [empty_sites]
    variable = empty_sites
    type = EmptySitesAux
    N = ${fparse N / cl}
    Ct0 = .1
    trap_per_free = ${trap_per_free}
    trapped_concentration_variables = trapped
  []
  [scaled_empty]
    variable = scaled_empty_sites
    type = NormalizationAux
    normal_factor = ${cl}
    source_variable = empty_sites
  []
  [trapped_sites]
    variable = trapped_sites
    type = NormalizationAux
    normal_factor = ${trap_per_free}
    source_variable = trapped
  []
  [total_sites]
    variable = total_sites
    type = ParsedAux
    function = 'trapped_sites + empty_sites'
    args = 'trapped_sites empty_sites'
  []
[]

[Kernels]
  [./diff]
    type = MatDiffusion
    variable = mobile
    diffusivity = ${fparse 1. / time_scaling}
    extra_vector_tags = ref
  [../]
  [./time]
    type = TimeDerivative
    variable = mobile
    extra_vector_tags = ref
  [../]
  [coupled_time]
    type = ScaledCoupledTimeDerivative
    variable = mobile
    v = trapped
    factor = ${trap_per_free}
    extra_vector_tags = ref
  []
[]

[NodalKernels]
  [time]
    type = TimeDerivativeNodalKernel
    variable = trapped
  []
  [trapping]
    type = TrappingNodalKernel
    variable = trapped
    alpha_t = ${fparse 1e15 / time_scaling}
    N = ${fparse 3.1622e22 / cl}
    Ct0 = 0.1
    mobile = 'mobile'
    trap_per_free = ${trap_per_free}
    extra_vector_tags = ref
  []
  [release]
    type = ReleasingNodalKernel
    alpha_r = ${fparse 1e13 / time_scaling}
    temp = 1000
    trapping_energy = ${epsilon}
    variable = trapped
  []
[]

[BCs]
  [left]
    type = DirichletBC
    variable = mobile
    value = ${fparse 3.1622e18 / cl}
    boundary = left
  []
  [right]
    type = DirichletBC
    variable = mobile
    value = 0
    boundary = right
  []
[]

[Postprocessors]
  [outflux]
    type = SideDiffusiveFluxAverage
    boundary = 'right'
    diffusivity = 1
    variable = mobile
  []
  [scaled_outflux]
    type = ScalePostprocessor
    value = outflux
    scaling_factor = ${cl}
  []
[]

[Preconditioning]
  [smp]
    type = SMP
    full = true
  []
[]

[Executioner]
  type = Transient
  end_time = 1000
  dt = 1
  solve_type = NEWTON
  line_search = 'none'
  automatic_scaling = true
  verbose = true
  compute_scaling_once = false
  l_tol = 1e-11
[]

[Outputs]
  exodus = true
  csv = true
  [dof]
    type = DOFMap
    execute_on = initial
  []
  perf_graph = true
[]
