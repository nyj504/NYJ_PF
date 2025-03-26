#pragma once

class SamplerState
{
public:
    SamplerState();
    ~SamplerState();

    void SetState(UINT slot = 0);

    void AddressU(D3D11_TEXTURE_ADDRESS_MODE mode) { desc.AddressU = mode; }
    void AddressV(D3D11_TEXTURE_ADDRESS_MODE mode) { desc.AddressV = mode; }
    void AddressW(D3D11_TEXTURE_ADDRESS_MODE mode) { desc.AddressW = mode; }

    void Filter(D3D11_FILTER value);

    void Changed();

private:
    D3D11_SAMPLER_DESC desc = {};
    ID3D11SamplerState* state = nullptr;
};